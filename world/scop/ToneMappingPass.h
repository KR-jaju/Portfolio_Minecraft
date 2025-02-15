#pragma once
#include "RenderPass.h"
#include "LightingPass.h"

class ToneMappingPass
{
public:
	struct Resources
	{
		ComPtr<ID3D11Texture2D> texture;
		ComPtr<ID3D11RenderTargetView> rtv;
		ComPtr<ID3D11ShaderResourceView> srv;
	};
	ToneMappingPass(ThreadPool& thread_pool);
	void initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager);
	void execute(ComPtr<ID3D11DeviceContext> const& context, LightingPass::Resources const& lighting);
	Resources const& getResources() const;
private:
	ThreadPool& thread_pool;
	ComPtr<ID3D11Device> device;
	Resources resources;

	ComPtr<ID3D11VertexShader> tone_mapping_vs;
	ComPtr<ID3D11PixelShader> tone_mapping_ps;
	ComPtr<ID3D11SamplerState> copy_sampler;

	void initializeTextures(ComPtr<ID3D11Device> const& device);
	void initializeData(ComPtr<ID3D11Device> const& device);
};