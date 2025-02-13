#include "pch.h"
#include "ToneMappingPass.h"

ToneMappingPass::ToneMappingPass() {}

void ToneMappingPass::execute(RenderingContext& context, RenderGroup const& render_group)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();

	this->bind(context);
	dc->Draw(4, 0);
	this->unbind(context);
}

void ToneMappingPass::initialize(RenderingContext& context, AssetManager& asset_manager)
{
	//	copy_sampler(context.graphics.getDevice()),
	this->initializeToneMappingShader(context, L"ToneMappingPassVS.hlsl", L"ToneMappingPassPS.hlsl");
	this->hdr_input = context.ping ? context.srvs["hdr_temporary[0]"] : context.srvs["hdr_temporary[1]"];
	this->ldr_output = context.ping ? context.rtvs["ldr_temporary[1]"] : context.rtvs["ldr_temporary[0]"];
	context.ping = !context.ping;
}

void ToneMappingPass::initializeToneMappingShader(RenderingContext& context, std::wstring const& vs_path, std::wstring const& ps_path)
{
	uint32 const compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	ComPtr<ID3DBlob> shader_blob;

	HRESULT hr = D3DCompileFromFile(vs_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreateVertexShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->tone_mapping_vs.GetAddressOf());
	CHECK(hr);
	hr = D3DCompileFromFile(ps_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreatePixelShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->tone_mapping_ps.GetAddressOf());
	CHECK(hr);
}

void ToneMappingPass::bind(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();
	ID3D11RenderTargetView* const* rtv = this->ldr_output.GetAddressOf();

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dc->VSSetShader(this->tone_mapping_vs.Get(), nullptr, 0);

	dc->PSSetShader(this->tone_mapping_ps.Get(), nullptr, 0);
	dc->PSSetSamplers(0, 1, this->copy_sampler.GetAddressOf());
	dc->PSSetShaderResources(0, 1, this->hdr_input.GetAddressOf());

	dc->OMSetRenderTargets(1, rtv, nullptr);
}

void ToneMappingPass::unbind(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();

	dc->OMSetRenderTargets(0, nullptr, nullptr);

	dc->PSSetShader(nullptr, nullptr, 0);

	dc->VSSetShader(nullptr, nullptr, 0);

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
}