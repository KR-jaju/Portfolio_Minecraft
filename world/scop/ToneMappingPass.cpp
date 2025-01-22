#include "pch.h"
#include "ToneMappingPass.h"

ToneMappingPass::ToneMappingPass(Renderer& renderer, RenderingContext& context)
	: tone_mapping_vs(context.graphics.getDevice(), L"ToneMappingPassVS.hlsl", "main", "vs_5_0"),
	tone_mapping_ps(context.graphics.getDevice(), L"ToneMappingPassPS.hlsl", "main", "ps_5_0"),
	copy_sampler(context.graphics.getDevice()),
	hdr_input(context.ping ? context.srvs["hdr_temporary[0]"] : context.srvs["hdr_temporary[1]"]),
	ldr_output(context.ping ? context.rtvs["ldr_temporary[1]"] : context.rtvs["ldr_temporary[0]"])
{
	context.ping = !context.ping;
}


void ToneMappingPass::execute(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();

	this->bind(context);
	dc->Draw(4, 0);
	this->unbind(context);
}

void ToneMappingPass::bind(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();
	ID3D11RenderTargetView* const* rtv = this->ldr_output.GetAddressOf();

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dc->VSSetShader(this->tone_mapping_vs.getComPtr().Get(), nullptr, 0);

	dc->PSSetShader(this->tone_mapping_ps.getComPtr().Get(), nullptr, 0);
	dc->PSSetSamplers(0, 1, this->copy_sampler.getComPtr().GetAddressOf());
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