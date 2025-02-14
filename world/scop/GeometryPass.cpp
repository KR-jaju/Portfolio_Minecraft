#include "pch.h"
#include "GeometryPass.h"
#include "RenderGroup.h"
#include "Player.h"
#include <iostream>



GeometryPass::GeometryPass() {}

void GeometryPass::execute(RenderingContext& context, RenderGroup const& render_group)
{
	ComPtr<ID3D11DeviceContext> dc = context.graphics.getContext();

	this->bind(context);
	dc->IASetInputLayout(context.chunk_input_layout.getComPtr().Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->VSSetShader(context.chunk_geometry_vs.getComPtr().Get(), nullptr, 0);
	dc->PSSetShader(context.chunk_geometry_ps.getComPtr().Get(), nullptr, 0);
	dc->PSSetSamplers(0, 1, context.chunk_texture_sampler.getComPtr().GetAddressOf());
	dc->PSSetShaderResources(0, 1, this->block_textures->getComPtr().GetAddressOf());

	for (RenderGroup::Subchunk const& subchunk : render_group.getSubchunks())
	{
		SubchunkMesh const& mesh = subchunk.mesh.get();
		D3D11_MAPPED_SUBRESOURCE subresource = {};
		HRESULT hr = dc->Map(this->subchunk_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		CHECK(hr);
		ivec3& position = *reinterpret_cast<ivec3*>(subresource.pData);
		
		position = ivec3(subchunk.x, subchunk.y, subchunk.z);
		dc->Unmap(this->subchunk_cb.Get(), 0);
		dc->VSSetConstantBuffers(1, 1, this->subchunk_cb.GetAddressOf());
		mesh.draw(context.graphics);
	}

	//for (auto& chunk_mesh : chunk_registry)
	//{
	//	ivec2 position = chunk_mesh.first;
	//	SubchunkMesh& mesh = chunk_mesh.second;

	//	this->renderer.render(mesh, position);
	//}
	dc->IASetInputLayout(context.entity_input_layout.getComPtr().Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->VSSetShader(context.entity_geometry_vs.getComPtr().Get(), nullptr, 0);
	dc->PSSetShader(context.entity_geometry_ps.getComPtr().Get(), nullptr, 0);
	//dc->PSSetSamplers(0, 1, context.entity_texture_sampler.getComPtr().GetAddressOf()); //TODO : WHAT?
	dc->PSSetShaderResources(0, 1, context.steve.getComPtr().GetAddressOf());
	//for (Entity& entity : entity_registry)
	//{
	//	renderer.render(entity);
	//}
	this->unbind(context);
}

void GeometryPass::initialize(RenderingContext& context, AssetManager& asset_manager)
{
	this->createAlbedoTexture(context);
	this->createPositionTexture(context);
	this->createNormalTexture(context);
	this->createDepthTexture(context);
	this->createSubchunkConstantBuffer(context);

	this->albedo_metallic_rtv = context.rtvs["gbuffer_albedo"];
	this->normal_rtv = context.rtvs["gbuffer_normal"];
	this->dsv = context.dsvs["gbuffer_depth"];
	this->block_textures = asset_manager.load<TextureArray>(L"./textures/block_textures.json");
}

void GeometryPass::bind(RenderingContext& context)
{
	D3D11_VIEWPORT const viewport = {
		0, 0, context.viewport_width, context.viewport_height, 0.0f, 1.0f
	};
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();
	ID3D11RenderTargetView* const albedo_metallic_rtv = this->albedo_metallic_rtv.Get();
	//ID3D11RenderTargetView* const position_rtv = this->position_rtv.Get();
	ID3D11RenderTargetView* const normal_rtv = this->normal_rtv.Get();
	ID3D11DepthStencilView* const dsv = this->dsv.Get();
	ID3D11RenderTargetView* const views[3] = {
		albedo_metallic_rtv,
		normal_rtv
	};
	float const albedo_clear[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	dc->ClearRenderTargetView(albedo_metallic_rtv, albedo_clear);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	dc->OMSetRenderTargets(2, views, dsv);
	dc->RSSetState(context.rasterizer_state.getComPtr().Get());
	dc->RSSetViewports(1, &viewport);


	//TODO : 다른 RTV들도 만들기!(RTV자체도 만들어야함)
}

void GeometryPass::unbind(RenderingContext& context)
{
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();

	dc->OMSetRenderTargets(0, nullptr, nullptr);
}



void	GeometryPass::createAlbedoTexture(RenderingContext& context)
{
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	int const width = context.viewport_width;
	int const height = context.viewport_height;
	ComPtr<ID3D11Texture2D>& texture = context.textures["gbuffer_albedo"];
	ComPtr<ID3D11RenderTargetView>& rtv = context.rtvs["gbuffer_albedo"];
	ComPtr<ID3D11ShaderResourceView>& srv = context.srvs["gbuffer_albedo"];
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = width;
	desc.Height = height;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.ArraySize = 1;
	desc.MipLevels = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = 0;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
	CHECK(hr);
	hr = device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
	CHECK(hr); // rtv만들기!
	hr = device->CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf());
	CHECK(hr);
}

void	GeometryPass::createPositionTexture(RenderingContext& context)
{
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	int const width = context.viewport_width;
	int const height = context.viewport_height;
	ComPtr<ID3D11Texture2D>& texture = context.textures["gbuffer_position"];
	ComPtr<ID3D11RenderTargetView>& rtv = context.rtvs["gbuffer_position"];
	ComPtr<ID3D11ShaderResourceView>& srv = context.srvs["gbuffer_position"];
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = width;
	desc.Height = height;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.ArraySize = 1;
	desc.MipLevels = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = 0;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
	CHECK(hr);
	hr = device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
	CHECK(hr); // rtv만들기!
	hr = device->CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf());
	CHECK(hr);
}

void	GeometryPass::createNormalTexture(RenderingContext& context)
{
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	int const width = context.viewport_width;
	int const height = context.viewport_height;
	ComPtr<ID3D11Texture2D>& texture = context.textures["gbuffer_normal"];
	ComPtr<ID3D11RenderTargetView>& rtv = context.rtvs["gbuffer_normal"];
	ComPtr<ID3D11ShaderResourceView>& srv = context.srvs["gbuffer_normal"];
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = width;
	desc.Height = height;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = DXGI_FORMAT_R16G16_FLOAT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.ArraySize = 1;
	desc.MipLevels = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = 0;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
	CHECK(hr);
	hr = device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
	CHECK(hr); // rtv만들기!
	hr = device->CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf());
	CHECK(hr);
}

void GeometryPass::createDepthTexture(RenderingContext& context)
{
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	int const width = context.viewport_width;
	int const height = context.viewport_height;
	ComPtr<ID3D11Texture2D>& texture = context.textures["gbuffer_depth"];
	ComPtr<ID3D11DepthStencilView>& dsv = context.dsvs["gbuffer_depth"];
	ComPtr<ID3D11ShaderResourceView>& srv = context.srvs["gbuffer_depth"];
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

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
	CHECK(hr);
	hr = device->CreateDepthStencilView(texture.Get(), &dsv_desc, dsv.GetAddressOf());
	CHECK(hr); // rtv만들기!
	hr = device->CreateShaderResourceView(texture.Get(), &srv_desc, srv.GetAddressOf());
	CHECK(hr);
}

void	GeometryPass::createSubchunkConstantBuffer(RenderingContext& context)
{
	Mat initial_data = Mat::identity();
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	ComPtr<ID3D11Buffer> subchunk_cb;
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = 16; // (x, y, z, padding)
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &initial_data;

	device->CreateBuffer(&desc, &data, subchunk_cb.GetAddressOf());
	this->subchunk_cb = std::move(subchunk_cb);
}

