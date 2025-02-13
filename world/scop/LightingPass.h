#pragma once

#include "RenderPass.h"
#include "VertexShader.h"
#include "Cubemap.h"

class LightingPass : public RenderPass
{
public:
	LightingPass();
	void execute(RenderingContext& context, RenderGroup const& render_group);
private:
	ComPtr<ID3D11ShaderResourceView> albedo_srv;
	ComPtr<ID3D11ShaderResourceView> normal_srv;
	ComPtr<ID3D11ShaderResourceView> depth_srv;

	ComPtr<ID3D11RenderTargetView> hdr_output;

	ComPtr<ID3D11VertexShader>	lighting_vs;
	ComPtr<ID3D11PixelShader>	lighting_ps;
	//SamplerState copy_sampler;

	//ConstantBuffer main_light_cb;

	void	initializeShaders(RenderingContext& context, std::wstring const& vs_path, std::wstring const& ps_path);

	void initialize(RenderingContext& context, AssetManager& asset_manager);
	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
};
