#include "pch.h"
#include "ShadowPass.h"
#include "InputLayouts.h"
#include "RenderGroup.h"

static ComPtr<ID3D11DepthStencilView> createDSV(RenderingContext& context, std::string const& name, int width, int height)
{
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	ComPtr<ID3D11Texture2D>& texture = context.textures[name];
	ComPtr<ID3D11DepthStencilView>& dsv = context.dsvs[name];
	ComPtr<ID3D11ShaderResourceView>& srv = context.srvs[name];
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

	HRESULT hr = device->CreateTexture2D(
		&desc, nullptr, texture.GetAddressOf());
	CHECK(hr);
	hr = device->CreateDepthStencilView(texture.Get(), &dsv_desc, dsv.GetAddressOf());
	CHECK(hr); // rtv만들기!
	hr = device->CreateShaderResourceView(texture.Get(), &srv_desc, srv.GetAddressOf());
	CHECK(hr);
	return (dsv);
}

ShadowPass::ShadowPass(RenderingContext& context)
	: context(context),
	shadow_caster_cb(context.graphics.getDevice(), context.graphics.getContext(), Mat::identity()),
	shadow_vs(context.graphics.getDevice(), L"ShadowPassVS.hlsl", "main", "vs_5_0"),
	shadow_il(context.graphics.getDevice(), InputLayouts::layout_chunk, 4, this->shadow_vs.getBlob())
{
	this->main_light_dsv[0] = createDSV(context, "main_shadow_map[0]", 1024, 1024);
	this->main_light_dsv[1] = createDSV(context, "main_shadow_map[1]", 1024, 1024);
	this->main_light_dsv[2] = createDSV(context, "main_shadow_map[2]", 1024, 1024);
	this->main_light_dsv[3] = createDSV(context, "main_shadow_map[3]", 1024, 1024);
}

void	ShadowPass::execute(RenderingContext& context, RenderGroup const& render_group)
{
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();
	//EntityRegistry& entity_registry = this->entity_registry;
	//LightRegistry& light_registry = this->light_registry;

	for (int level = 0; level < 4; ++level)
	{
		ComPtr<ID3D11DepthStencilView> dsv = this->main_light_dsv[level].Get();

		//this->shadow_caster_cb.update(light_registry.main_light.getViewProjectionMatrix(level));

		dc->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		dc->OMSetRenderTargets(0, nullptr, dsv.Get());

		dc->IASetInputLayout(this->shadow_il.getComPtr().Get());
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->VSSetShader(this->shadow_vs.getComPtr().Get(), nullptr, 0);
		dc->VSSetConstantBuffers(0, 1, this->shadow_caster_cb.getComPtr().GetAddressOf());
		//for (auto& chunk_mesh : chunk_mesh_registry)
		//{
		//	ivec2 position = chunk_mesh.first;
		//	SubchunkMeshData& mesh = chunk_mesh.second;

		//	this->renderer.render(mesh, position);
		//}
	}
}


void ShadowPass::bind(RenderingContext& context)
{
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();
	D3D11_VIEWPORT const viewport = {
		0, 0, 1024, 1024, 0.0f, 1.0f
	};

	dc->RSSetState(context.rasterizer_state.getComPtr().Get());
	dc->RSSetViewports(1, &viewport);
}

void ShadowPass::unbind(RenderingContext& context)
{
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();

	dc->OMSetRenderTargets(0, nullptr, nullptr);
}
