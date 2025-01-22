#pragma once

#include "RenderPass.h"
#include "VertexShader.h"
#include "Cubemap.h"

class LightingPass : public RenderPass
{
public:
	LightingPass(Renderer& renderer, RenderingContext& context);
	void execute(RenderingContext& context);
private:
	ComPtr<ID3D11ShaderResourceView> albedo_srv;
	ComPtr<ID3D11ShaderResourceView> normal_srv;
	ComPtr<ID3D11ShaderResourceView> depth_srv;

	ComPtr<ID3D11RenderTargetView> hdr_output;

	VertexShader	lighting_vs;
	PixelShader	lighting_ps;
	SamplerState copy_sampler;

	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
};
