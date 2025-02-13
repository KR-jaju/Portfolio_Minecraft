#pragma once
#include "RenderPass.h"

class ToneMappingPass : public RenderPass
{
public:
	ToneMappingPass();
	void execute(RenderingContext& context, RenderGroup const& render_group);
private:
	//VertexShader	tone_mapping_vs;
	//PixelShader	tone_mapping_ps;
	//SamplerState copy_sampler;

	ComPtr<ID3D11VertexShader> tone_mapping_vs;
	ComPtr<ID3D11PixelShader> tone_mapping_ps;
	ComPtr<ID3D11SamplerState> copy_sampler;
	ComPtr<ID3D11ShaderResourceView> hdr_input;
	ComPtr<ID3D11RenderTargetView> ldr_output;

	void initialize(RenderingContext& context, AssetManager& asset_manager);
	void initializeToneMappingShader(RenderingContext& context, std::wstring const& vs_path, std::wstring const& ps_path);

	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
};