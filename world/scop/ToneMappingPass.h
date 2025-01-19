#pragma once
#include "RenderPass.h"

class ToneMappingPass : public RenderPass
{
public:
	ToneMappingPass(Renderer& renderer, RenderingContext& context);
	void execute(RenderingContext& context);
private:
	VertexShader	tone_mapping_vs;
	PixelShader	tone_mapping_ps;
	SamplerState copy_sampler;

	ComPtr<ID3D11ShaderResourceView> hdr_input;
	ComPtr<ID3D11RenderTargetView> ldr_output;

	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
};