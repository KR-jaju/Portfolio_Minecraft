#pragma once

#include "ChunkRegistry.h"
#include "Graphics.h"

#include "PixelShader.h"
#include "VertexShader.h"
#include "RasterizerState.h"
#include "InputLayout.h"
#include "SamplerState.h"

#include <unordered_map>

#include "LightRegistry.h"

#include "Texture.h"

struct RenderingContext
{
	RenderingContext(AssetManager& asset_manager, Graphics& graphics, int width, int height);

	AssetManager& asset_manager;
	Graphics& graphics;

	int viewport_width;
	int viewport_height;

	RasterizerState rasterizer_state;

	Texture steve;

	CameraMatrices camera_matrices;
	ConstantBuffer camera_data;

	ConstantBuffer light_cb;

	VertexShader chunk_geometry_vs;
	PixelShader chunk_geometry_ps;

	VertexShader entity_geometry_vs;
	PixelShader entity_geometry_ps;
	
	SamplerState chunk_texture_sampler;

	InputLayout chunk_input_layout;
	InputLayout entity_input_layout;

	std::unordered_map<std::string, ComPtr<ID3D11Texture2D>> textures;
	std::unordered_map<std::string, ComPtr<ID3D11RenderTargetView>> rtvs;
	std::unordered_map<std::string, ComPtr<ID3D11DepthStencilView>> dsvs;
	std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> srvs;

	bool	ping;
private:
	void	prepareTemporaryTextures();
};