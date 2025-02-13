#include "pch.h"
#include "LightingPass.h"
#include <iostream>

LightingPass::LightingPass() {}

void LightingPass::execute(RenderingContext& context, RenderGroup const& render_group)
{
	//LightRegistry& light_registry = context.light_registry;
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();
	//DirectionalLightData light = light_registry.main_light_data;

	this->bind(context);
	//this->main_light_cb.update(light);
	//dc->PSSetConstantBuffers(1, 1, this->main_light_cb.getComPtr().GetAddressOf());
	dc->Draw(4, 0);
	//this->unbind(context);
}

void	LightingPass::initializeShaders(RenderingContext& context, std::wstring const& vs_path, std::wstring const& ps_path)
{
	uint32 const compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	ComPtr<ID3DBlob> shader_blob;

	HRESULT hr = D3DCompileFromFile(vs_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreateVertexShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->lighting_vs.GetAddressOf());
	CHECK(hr);
	hr = D3DCompileFromFile(ps_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreatePixelShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->lighting_ps.GetAddressOf());
	CHECK(hr);
}

void LightingPass::initialize(RenderingContext& context, AssetManager& asset_manager)
{
	this->albedo_srv = context.srvs["gbuffer_albedo"];
	this->depth_srv = context.srvs["gbuffer_depth"];
	this->normal_srv = context.srvs["gbuffer_normal"];
	this->hdr_output = context.ping ? context.rtvs["hdr_temporary[0]"] : context.rtvs["hdr_temporary[1]"];

	this->initializeShaders(context, L"LightingPassVS.hlsl", L"LightingPassPS.hlsl");
	//lighting_vs(context.graphics.getDevice(), L"LightingPassVS.hlsl", "main", "vs_5_0"),
	//lighting_ps(context.graphics.getDevice(), L"LightingPassPS.hlsl", "main", "ps_5_0"),
	//copy_sampler(context.graphics.getDevice()),
	//main_light_cb(context.graphics.getDevice(), context.graphics.getContext(), DirectionalLightData{})
}

void LightingPass::bind(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();
	ID3D11RenderTargetView* const* rtv = this->hdr_output.GetAddressOf();
	ID3D11ShaderResourceView* gbuffer_srv[] = {
		this->albedo_srv.Get(),
		this->normal_srv.Get(),
		this->depth_srv.Get()
	};

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dc->IASetInputLayout(nullptr);

	dc->VSSetShader(this->lighting_vs.Get(), nullptr, 0);

	dc->PSSetShader(this->lighting_ps.Get(), nullptr, 0);
	dc->PSSetShaderResources(0, 3, gbuffer_srv); // TODO : 지금은 2개이지만 !!!
	dc->PSSetConstantBuffers(0, 1, context.camera_data.getComPtr().GetAddressOf());

	dc->OMSetRenderTargets(1, rtv, nullptr);
}
void LightingPass::unbind(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();

	dc->OMSetRenderTargets(0, nullptr, nullptr);

	dc->PSSetShader(nullptr, nullptr, 0);

	dc->VSSetShader(nullptr, nullptr, 0);

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
}
