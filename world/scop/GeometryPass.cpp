#include "pch.h"
#include "GeometryPass.h"
#include "Player.h"
#include <iostream>

static ComPtr<ID3D11RenderTargetView>	createAlbedoRTV(RenderingContext& context)
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
	return (rtv);
}

static ComPtr<ID3D11DepthStencilView> createDSV(RenderingContext& context)
{
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	int const width = context.viewport_width;
	int const height = context.viewport_height;
	ComPtr<ID3D11Texture2D> texture = context.textures["gbuffer_depth"];
	ComPtr<ID3D11DepthStencilView> dsv = context.dsvs["gbuffer_depth"];
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(
		&desc, nullptr, texture.GetAddressOf());
	CHECK(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};

	dsv_desc.Format = desc.Format;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(texture.Get(), &dsv_desc, dsv.GetAddressOf());
	CHECK(hr); // rtv만들기!
	return (dsv);
}



GeometryPass::GeometryPass(Renderer& renderer, RenderingContext& context)
	: renderer(renderer),
	albedo_rtv(createAlbedoRTV(context)),
	dsv(createDSV(context)),
	model_cb(context.graphics.getDevice(), context.graphics.getContext(), Mat::Identity),
	armature_cb(context.graphics.getDevice(), context.graphics.getContext(), Mat::Identity)
{

}

void GeometryPass::execute(RenderingContext& context)
{
	TextureRegistry& texture_registry = context.texture_registry;
	std::pair<int, int> center = context.center;
	int render_distance = context.render_distance;
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();

	ChunkMeshRegistry& chunk_mesh_registry = context.chunk_mesh_registry;
	EntityRegistry& entity_registry = context.entity_registry;

	this->bind(context);
	dc->IASetInputLayout(context.chunk_input_layout.getComPtr().Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->VSSetShader(context.chunk_geometry_vs.getComPtr().Get(), nullptr, 0);
	dc->PSSetShader(context.chunk_geometry_ps.getComPtr().Get(), nullptr, 0);
	dc->VSSetConstantBuffers(0, 1, context.camera_data.getComPtr().GetAddressOf());
	dc->PSSetSamplers(0, 1, context.chunk_texture_sampler.getComPtr().GetAddressOf());
	dc->PSSetShaderResources(0, 1, texture_registry.getBlockTextureArray().GetAddressOf());

	for (auto& chunk_mesh : chunk_mesh_registry)
	{
		ivec3 position = chunk_mesh.first;
		SubchunkMeshData& mesh = chunk_mesh.second;

		this->renderer.render(mesh, position);
	}
	dc->IASetInputLayout(context.entity_input_layout.getComPtr().Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->VSSetShader(context.entity_geometry_vs.getComPtr().Get(), nullptr, 0);
	dc->PSSetShader(context.entity_geometry_ps.getComPtr().Get(), nullptr, 0);
	dc->VSSetConstantBuffers(0, 1, context.camera_data.getComPtr().GetAddressOf());
	//dc->PSSetSamplers(0, 1, context.entity_texture_sampler.getComPtr().GetAddressOf());
	//dc->PSSetShaderResources(0, 1, texture_registry.getBlockTextureArray().GetAddressOf());
	for (Entity& entity : entity_registry)
	{
		renderer.render(entity);
	}
	this->unbind(context);
}

void GeometryPass::bind(RenderingContext& context)
{
	D3D11_VIEWPORT const viewport = {
		0, 0, context.viewport_width, context.viewport_height, 0.0f, 1.0f
	};
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();
	ID3D11RenderTargetView* const albedo_rtv = this->albedo_rtv.Get();
	ID3D11DepthStencilView* const dsv = this->dsv.Get();
	ID3D11RenderTargetView* const views[4] = {
		albedo_rtv
	};
	float const albedo_clear[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	dc->ClearRenderTargetView(albedo_rtv, albedo_clear);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	dc->OMSetRenderTargets(4, views, dsv);
	dc->RSSetState(context.rasterizer_state.getComPtr().Get());
	dc->RSSetViewports(1, &viewport);


	//TODO : 다른 RTV들도 만들기!(RTV자체도 만들어야함)
}

void GeometryPass::unbind(RenderingContext& context)
{
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();

	dc->OMSetRenderTargets(0, nullptr, nullptr);
}
