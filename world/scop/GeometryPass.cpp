#include "pch.h"
#include "GeometryPass.h"
#include "RenderGroup.h"
#include "Player.h"
#include <iostream>
#include <functional>
#include "InputLayouts.h"

GeometryPass::GeometryPass(ThreadPool& thread_pool)
	: thread_pool(thread_pool) {}

std::shared_future<void> GeometryPass::dispatch(
	ComPtr<ID3D11DeviceContext> const& immediate_context,
	std::mutex& context_mutex,
	std::shared_future<void> const& visibility_ready,
	RenderGroup const& visible_group)
{
	std::promise<void> result_promise;
	std::shared_future<void> result_future = result_promise.get_future().share();

	this->thread_pool.enqueue(ThreadPool::Priority::Immediate, [immediate_context, &context_mutex, this, visibility_ready, visible_group]() {
		thread_local ComPtr<ID3D11DeviceContext> deferred_context = nullptr;
		ComPtr<ID3D11CommandList> command_list = nullptr;

		if (deferred_context == nullptr)
		{
			HRESULT hr = device->CreateDeferredContext(0, deferred_context.GetAddressOf());

			CHECK(hr);
		}
		visibility_ready.get();
		this->execute(deferred_context, visible_group);
		deferred_context->FinishCommandList(TRUE, command_list.GetAddressOf()); // Finish & clear
		std::unique_lock<std::mutex> context_lock(context_mutex);
		immediate_context->ExecuteCommandList(command_list.Get(), FALSE);
		});
	return (result_future);
}

void GeometryPass::execute(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& render_group)
{
	D3D11_VIEWPORT const viewport = { 0, 0, 800, 800, 0, 1 };
		//context.viewport_width, context.viewport_height, 0.0f, 1.0f };
	ID3D11RenderTargetView* const views[3] = {
		this->resources.rtv[0].Get(),
		this->resources.rtv[1].Get()
	};
	float const albedo_clear[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	context->ClearRenderTargetView(this->resources.rtv[0].Get(), albedo_clear);
	context->ClearDepthStencilView(this->resources.depth_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->RSSetState(this->subchunk_rs.Get());
	context->RSSetViewports(1, &viewport);
	context->IASetInputLayout(this->subchunk_il.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(this->subchunk_vs.Get(), nullptr, 0);
	context->PSSetShader(this->subchunk_ps.Get(), nullptr, 0);
	context->PSSetSamplers(0, 1, this->subchunk_ss.GetAddressOf());
	context->PSSetShaderResources(0, 1, this->block_textures->getComPtr().GetAddressOf());
	context->OMSetRenderTargets(2, views, this->resources.depth_dsv.Get());

	for (RenderGroup::Subchunk const& subchunk : render_group.getSubchunks())
	{
		SubchunkMesh const& mesh = subchunk.mesh.get();
		D3D11_MAPPED_SUBRESOURCE subresource = {};
		HRESULT hr = context->Map(this->subchunk_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		CHECK(hr);
		ivec3& position = *reinterpret_cast<ivec3*>(subresource.pData);
		
		position = ivec3(subchunk.x, subchunk.y, subchunk.z);
		context->Unmap(this->subchunk_cb.Get(), 0);
		context->VSSetConstantBuffers(1, 1, this->subchunk_cb.GetAddressOf());
		mesh.draw(context);
	}

	//for (auto& chunk_mesh : chunk_registry)
	//{
	//	ivec2 position = chunk_mesh.first;
	//	SubchunkMesh& mesh = chunk_mesh.second;

	//	this->renderer.render(mesh, position);
	//}
	//dc->IASetInputLayout(context.entity_input_layout.getComPtr().Get());
	//dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//dc->VSSetShader(context.entity_geometry_vs.getComPtr().Get(), nullptr, 0);
	//dc->PSSetShader(context.entity_geometry_ps.getComPtr().Get(), nullptr, 0);
	////dc->PSSetSamplers(0, 1, context.entity_texture_sampler.getComPtr().GetAddressOf()); //TODO : WHAT?
	//dc->PSSetShaderResources(0, 1, context.steve.getComPtr().GetAddressOf());
	//for (Entity& entity : entity_registry)
	//{
	//	renderer.render(entity);
	//}
	context->OMSetRenderTargets(0, nullptr, nullptr);
}

GeometryPass::Resources const& GeometryPass::getResources() const
{
	return (this->resources);
}

void GeometryPass::initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager)
{
	this->initializeTextures(device);
	this->initializeSubchunkData(device);
	this->device = device;
	this->block_textures = asset_manager.load<TextureArray>(L"./textures/block_textures.json");
}


void	GeometryPass::initializeTextures(ComPtr<ID3D11Device> const& device)
{
	int const width = 800;
	int const height = 800;
	ComPtr<ID3D11Texture2D> texture_holder;

	{// Depth-Stencil
		ComPtr<ID3D11DepthStencilView>& dsv = this->resources.depth_dsv;
		ComPtr<ID3D11ShaderResourceView>& srv = this->resources.depth_srv;
		D3D11_TEXTURE2D_DESC desc = {};
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Texture2D.MipSlice = 0;

		srv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; // SRV용 포맷
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.MipLevels = 1;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture_holder.GetAddressOf());
		CHECK(hr);
		hr = device->CreateDepthStencilView(texture_holder.Get(), &dsv_desc, dsv.GetAddressOf());
		CHECK(hr); // rtv만들기!
		hr = device->CreateShaderResourceView(texture_holder.Get(), &srv_desc, srv.GetAddressOf());
		CHECK(hr);
	}
	{// Albedo and metallic
		ComPtr<ID3D11RenderTargetView>& rtv = this->resources.rtv[0];
		ComPtr<ID3D11ShaderResourceView>& srv = this->resources.srv[0];
		D3D11_TEXTURE2D_DESC desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture_holder.GetAddressOf());
		CHECK(hr);
		hr = device->CreateRenderTargetView(texture_holder.Get(), nullptr, rtv.GetAddressOf());
		CHECK(hr);
		hr = device->CreateShaderResourceView(texture_holder.Get(), nullptr, srv.GetAddressOf());
		CHECK(hr);
	}
	{// View-space Normal
		ComPtr<ID3D11RenderTargetView>& rtv = this->resources.rtv[1];
		ComPtr<ID3D11ShaderResourceView>& srv = this->resources.srv[1];
		D3D11_TEXTURE2D_DESC desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R16G16_FLOAT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture_holder.GetAddressOf());
		CHECK(hr);
		hr = device->CreateRenderTargetView(texture_holder.Get(), nullptr, rtv.GetAddressOf());
		CHECK(hr);
		hr = device->CreateShaderResourceView(texture_holder.Get(), nullptr, srv.GetAddressOf());
		CHECK(hr);
	}
}
//void	GeometryPass::createPositionTexture(RenderingContext& context)
//{
//	ComPtr<ID3D11Device> device = context.graphics.getDevice();
//	int const width = context.viewport_width;
//	int const height = context.viewport_height;
//	ComPtr<ID3D11Texture2D>& texture = context.textures["gbuffer_position"];
//	ComPtr<ID3D11RenderTargetView>& rtv = context.rtvs["gbuffer_position"];
//	ComPtr<ID3D11ShaderResourceView>& srv = context.srvs["gbuffer_position"];
//	D3D11_TEXTURE2D_DESC desc = {};
//
//	desc.Width = width;
//	desc.Height = height;
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
//	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//	desc.ArraySize = 1;
//	desc.MipLevels = 0;
//	desc.SampleDesc.Count = 1;
//	desc.SampleDesc.Quality = 0;
//	desc.CPUAccessFlags = 0;
//
//	HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
//	CHECK(hr);
//	hr = device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
//	CHECK(hr); // rtv만들기!
//	hr = device->CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf());
//	CHECK(hr);
//}

void	GeometryPass::initializeSubchunkData(ComPtr<ID3D11Device> const& device)
{
	{// VS, PS, IL
		uint32 const compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		ComPtr<ID3DBlob> blob;

		HRESULT hr = D3DCompileFromFile(L"GeometryPassVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compile_flag, 0, blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, this->subchunk_vs.GetAddressOf());
		CHECK(hr);
		hr = device->CreateInputLayout(InputLayouts::layout_chunk, 4, blob->GetBufferPointer(), blob->GetBufferSize(), this->subchunk_il.GetAddressOf());
		CHECK(hr);
		hr = D3DCompileFromFile(L"GeometryPassPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compile_flag, 0, blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, this->subchunk_ps.GetAddressOf());
		CHECK(hr);
	}
	{// CB
		Mat const initial_data = Mat::identity();
		ComPtr<ID3D11Buffer> subchunk_cb;
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA data = {};

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = 16; // (x, y, z, padding)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		data.pSysMem = &initial_data;
		device->CreateBuffer(&desc, &data, this->subchunk_cb.GetAddressOf());
	}
	{// RS
		D3D11_RASTERIZER_DESC desc = {};

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		HRESULT hr = device->CreateRasterizerState(&desc, this->subchunk_rs.GetAddressOf());
		CHECK(hr);
	}
	{// Sampler state
		D3D11_SAMPLER_DESC desc = {};

		desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = device->CreateSamplerState(&desc, this->subchunk_ss.GetAddressOf());
		CHECK(hr);
	}
}
