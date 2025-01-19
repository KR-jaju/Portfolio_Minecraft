#include "pch.h"
#include "LightingPass.h"
#include <iostream>

LightingPass::LightingPass(Renderer& renderer, RenderingContext& context)
	: lighting_vs(context.graphics.getDevice(), L"LightingPassVS.hlsl", "main", "vs_5_0"),
	lighting_ps(context.graphics.getDevice(), L"LightingPassPS.hlsl", "main", "ps_5_0"),
	copy_sampler(context.graphics.getDevice()),
	albedo_srv(context.srvs["gbuffer_albedo"]),
	hdr_output(context.ping ? context.rtvs["hdr_temporary[0]"] : context.rtvs["hdr_temporary[1]"])
{
	std::cout << albedo_srv.Get();
}

void LightingPass::execute(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();

	this->bind(context);
	dc->Draw(4, 0);
	this->unbind(context);
}

void LightingPass::bind(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();
	ID3D11RenderTargetView* const* rtv = this->hdr_output.GetAddressOf();
	ID3D11ShaderResourceView* gbuffer_srv[] = {
		this->albedo_srv.Get()
	};

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	dc->VSSetShader(this->lighting_vs.getComPtr().Get(), nullptr, 0);

	dc->PSSetShader(this->lighting_ps.getComPtr().Get(), nullptr, 0);
	dc->PSSetSamplers(0, 1, this->copy_sampler.getComPtr().GetAddressOf());
	dc->PSSetShaderResources(0, 1, gbuffer_srv); // TODO : 지금은 1개이지만 !!!

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