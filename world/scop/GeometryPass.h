#pragma once

#include "RenderPass.h"
#include "TextureArray.h"
#include "VisibilityProcessor.h"

class GeometryPass
{
public:
	struct Resources
	{
		ComPtr<ID3D11RenderTargetView> rtv[2];
		ComPtr<ID3D11ShaderResourceView> srv[2];
		ComPtr<ID3D11DepthStencilView> depth_dsv;
		ComPtr<ID3D11ShaderResourceView> depth_srv;
	};

	GeometryPass(ThreadPool& thread_pool);

	void initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager);
	
	std::shared_future<void> dispatch(
		ComPtr<ID3D11DeviceContext> const& immediate_context,
		std::mutex& context_mutex,
		std::shared_future<void> const& visibility_ready,
		RenderGroup const& visible_group,
		ComPtr<ID3D11Buffer> const& camera_info
	);

	void execute(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& render_group, ComPtr<ID3D11Buffer> const& camera_info);
	Resources const& getResources() const;
private:
	ThreadPool& thread_pool;
	ComPtr<ID3D11Device> device;
	Resources resources;

	ComPtr<ID3D11VertexShader> subchunk_vs;
	ComPtr<ID3D11PixelShader> subchunk_ps;
	ComPtr<ID3D11InputLayout> subchunk_il;
	ComPtr<ID3D11Buffer> subchunk_cb;
	ComPtr<ID3D11SamplerState> subchunk_ss;
	ComPtr<ID3D11RasterizerState> subchunk_rs;

	std::shared_ptr<TextureArray> block_textures;

	void	initializeTextures(ComPtr<ID3D11Device> const& device);
	void	initializeSubchunkData(ComPtr<ID3D11Device> const& device);
};