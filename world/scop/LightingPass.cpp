#include "pch.h"
#include "LightingPass.h"
#include <iostream>
#include "InputLayouts.h"

static std::vector<vec3>	createSphereVertices(int subdivision)
{
	std::vector<vec3> vertices;

	for (int v = 0; v <= subdivision; ++v)
	{
		for (int u = 0; u <= subdivision; ++u)
		{
			vertices.emplace_back(1, 1 - 2 * (float)v / subdivision, 1 - 2 * (float)u / subdivision); // east
			vertices.back().Normalize();
			vertices.emplace_back(-1, 1 - 2 * (float)v / subdivision, -1 + 2 * (float)u / subdivision); // west
			vertices.back().Normalize();
			vertices.emplace_back(1 - 2 * (float)u / subdivision, 1, 1 - 2 * (float)v / subdivision); // up
			vertices.back().Normalize();
			vertices.emplace_back(1 - 2 * (float)u / subdivision, -1, -1 + 2 * (float)v / subdivision); // down
			vertices.back().Normalize();
			vertices.emplace_back(-1 + 2 * (float)u / subdivision, 1 - 2 * (float)v / subdivision, 1); // north
			vertices.back().Normalize();
			vertices.emplace_back(1 - 2 * (float)u / subdivision, 1 - 2 * (float)v / subdivision, -1); // south
			vertices.back().Normalize();
		}
	}
	return (vertices);
}
static std::vector<uint32>	createSphereIndices(int subdivision)
{
	std::vector<uint32> indices;

	for (int v = 0; v < subdivision; ++v)
	{
		for (int u = 0; u < subdivision; ++u)
		{
			for (int offset = 0; offset < 6; ++offset)
			{
				indices.emplace_back((u * (subdivision + 1) + v) * 6 + offset);
				indices.emplace_back(((u + 1) * (subdivision + 1) + v) * 6 + offset);
				indices.emplace_back((u * (subdivision + 1) + (v + 1)) * 6 + offset);

				indices.emplace_back(((u + 1) * (subdivision + 1) + v) * 6 + offset);
				indices.emplace_back(((u + 1) * (subdivision + 1) + (v + 1)) * 6 + offset);
				indices.emplace_back((u * (subdivision + 1) + (v + 1)) * 6 + offset);
			}
		}
	}
	return (indices);
}

LightingPass::LightingPass(ThreadPool& thread_pool)
	: thread_pool(thread_pool) {}

std::shared_future<void> LightingPass::dispatch(
	ComPtr<ID3D11DeviceContext> const& immediate_context,
	std::mutex& context_mutex,
	std::shared_future<void> const& geometry_ready,
	GeometryPass::Resources const& geometry_result,
	ComPtr<ID3D11Buffer> const& camera_info
)
{
	std::promise<void> result_promise;
	std::shared_future<void> result_future = result_promise.get_future().share();

	this->thread_pool.enqueue(ThreadPool::Priority::Immediate, [immediate_context, &context_mutex, promise = std::move(result_promise), this, geometry_ready, geometry_result, camera_info]() mutable {
		thread_local ComPtr<ID3D11DeviceContext> deferred_context = nullptr;
		ComPtr<ID3D11CommandList> command_list = nullptr;

		if (deferred_context == nullptr)
		{
			HRESULT hr = device->CreateDeferredContext(0, deferred_context.GetAddressOf());

			CHECK(hr);
		}
		this->execute(deferred_context, geometry_result, camera_info);
		deferred_context->FinishCommandList(TRUE, command_list.GetAddressOf()); // Finish & clear
		{
			geometry_ready.get(); // Geometry Pass ´ë±â
			std::unique_lock<std::mutex> context_lock(context_mutex);
			immediate_context->ExecuteCommandList(command_list.Get(), FALSE);
		}
		promise.set_value();
		});
	return (result_future);
}

void LightingPass::execute(ComPtr<ID3D11DeviceContext> const& context, GeometryPass::Resources const& geometry, ComPtr<ID3D11Buffer> const& camera_info)
{
	ID3D11ShaderResourceView* const gbuffer_srv[] = {
		geometry.srv[0].Get(),
		geometry.srv[1].Get(),
		geometry.depth_srv.Get()
	};
	uint32 stride = sizeof(vec3);
	uint32 offset = 0;

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetInputLayout(nullptr);
	context->VSSetShader(this->lighting_vs.Get(), nullptr, 0);
	context->VSSetConstantBuffers(0, 1, camera_info.GetAddressOf());
	context->PSSetShader(this->lighting_ps.Get(), nullptr, 0);
	context->PSSetConstantBuffers(0, 1, camera_info.GetAddressOf());
	context->PSSetShaderResources(0, 3, gbuffer_srv);
	context->OMSetRenderTargets(1, this->resources.rtv.GetAddressOf(), nullptr);
	context->Draw(4, 0);

	context->IASetInputLayout(this->sky_il.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, this->sky_vertex_buffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->sky_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(this->sky_vs.Get(), nullptr, 0);
	context->PSSetShader(this->sky_ps.Get(), nullptr, 0);
	context->PSSetSamplers(0, 1, this->sky_ss.GetAddressOf());
	context->PSSetShaderResources(0, 1, this->sky_texture->getComPtr().GetAddressOf());
	context->OMSetRenderTargets(1, this->resources.rtv.GetAddressOf(), geometry.depth_dsv.Get());
	context->DrawIndexed(this->sky_indices.size(), 0, 0);

	context->OMSetRenderTargets(0, nullptr, nullptr);
}

LightingPass::Resources const& LightingPass::getResources() const
{
	return (this->resources);
}

void LightingPass::initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager)
{
	this->intializeTextures(device);
	this->initializeLightingData(device);
	this->initializeSkyData(device);
	this->sky_texture = asset_manager.load<Cubemap>(L"./textures/skybox/HDRI/MyCubesEnvHDR.dds");
}

void LightingPass::intializeTextures(ComPtr<ID3D11Device> const& device)
{
	int const width = 800;
	int const height = 800;
	ComPtr<ID3D11Texture2D> texture_holder;

	{// Result
		ComPtr<ID3D11RenderTargetView>& rtv = this->resources.rtv;
		ComPtr<ID3D11ShaderResourceView>& srv = this->resources.srv;
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
		hr = device->CreateRenderTargetView(texture_holder.Get(), nullptr, this->resources.rtv.GetAddressOf());
		CHECK(hr);
		hr = device->CreateShaderResourceView(texture_holder.Get(), nullptr, this->resources.srv.GetAddressOf());
		CHECK(hr);
	}
}

void	LightingPass::initializeLightingData(ComPtr<ID3D11Device> const& device)
{
	uint32 const compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ComPtr<ID3DBlob> shader_blob;

	HRESULT hr = D3DCompileFromFile(L"LightingPassVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreateVertexShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->lighting_vs.GetAddressOf());
	CHECK(hr);
	hr = D3DCompileFromFile(L"LightingPassPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreatePixelShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->lighting_ps.GetAddressOf());
	CHECK(hr);
}

void LightingPass::initializeSkyData(ComPtr<ID3D11Device> const& device)
{
	{ // Vertex shader, Pixel shader, Input layout
		uint32 const compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		ComPtr<ID3DBlob> shader_blob;

		HRESULT hr = D3DCompileFromFile(L"SkyPassVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreateVertexShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->sky_vs.GetAddressOf());
		CHECK(hr);
		hr = device->CreateInputLayout(InputLayouts::layout_skybox, 1, shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), this->sky_il.GetAddressOf());
		CHECK(hr);
		hr = D3DCompileFromFile(L"SkyPassPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreatePixelShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->sky_ps.GetAddressOf());
		CHECK(hr);
	}
	{ // Vertex, index data
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA data = {};

		this->sky_vertices = std::move(createSphereVertices(3));
		this->sky_indices = std::move(createSphereIndices(3));
		desc.ByteWidth = sizeof(vec3) * this->sky_vertices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = this->sky_vertices.data();
		HRESULT hr = device->CreateBuffer(&desc, &data, this->sky_vertex_buffer.GetAddressOf());
		CHECK(hr);
		desc.ByteWidth = sizeof(uint32) * this->sky_indices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		data.pSysMem = this->sky_indices.data();
		hr = device->CreateBuffer(&desc, &data, this->sky_index_buffer.GetAddressOf());
		CHECK(hr);
	}
	{ // Sampler state
		D3D11_SAMPLER_DESC desc = {};

		desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = device->CreateSamplerState(&desc, this->sky_ss.GetAddressOf());
		CHECK(hr);
	}
}
