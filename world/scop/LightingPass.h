#pragma once

#include "RenderPass.h"
#include "VertexShader.h"

class LightingPass : public RenderPass
{
public:
	LightingPass(Renderer& renderer, RenderingContext& context);
	void execute(RenderingContext& context);
private:
	VertexShader	lighting_vs;
	PixelShader	lighting_ps;
	SamplerState copy_sampler;
	
	ComPtr<ID3D11ShaderResourceView> albedo_srv;

	ComPtr<ID3D11RenderTargetView> hdr_output;

	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
};
