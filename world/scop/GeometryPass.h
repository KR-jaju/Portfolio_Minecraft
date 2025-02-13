#pragma once

#include "RenderPass.h"
#include "TextureArray.h"

class GeometryPass : public RenderPass
{
public:
	GeometryPass();
	void execute(RenderingContext& context, RenderGroup const& render_group);
private:
	ComPtr<ID3D11RenderTargetView> albedo_metallic_rtv;
	ComPtr<ID3D11RenderTargetView> normal_rtv;
	ComPtr<ID3D11DepthStencilView> dsv;

	ComPtr<ID3D11Buffer> subchunk_cb;

	std::shared_ptr<TextureArray> block_textures;

	void initialize(RenderingContext& context, AssetManager& asset_manager);
	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);

	void	createAlbedoTexture(RenderingContext& context);
	void	createPositionTexture(RenderingContext& context);
	void	createNormalTexture(RenderingContext& context);
	void	createDepthTexture(RenderingContext& context);
	void	createSubchunkConstantBuffer(RenderingContext& context);
};