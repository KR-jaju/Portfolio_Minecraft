#pragma once

#include "ChunkRegistry.h"
#include "Graphics.h"

#include "PixelShader.h"
#include "VertexShader.h"
#include "RasterizerState.h"
#include "InputLayout.h"
#include "SamplerState.h"

#include <unordered_map>

#include "ChunkMeshRegistry.h"
#include "LRUCache.h"

struct RenderingContext
{
	RenderingContext(TextureRegistry& texture_registry, EntityRegistry& entity_registry, ChunkMeshRegistry& chunk_mesh_registry, Graphics& graphics, int width, int height);

	TextureRegistry& texture_registry;
	EntityRegistry& entity_registry;
	ChunkMeshRegistry& chunk_mesh_registry;
	Graphics& graphics;

	std::pair<int, int> center;
	int	render_distance;

	int viewport_width;
	int viewport_height;

	RasterizerState rasterizer_state;

	ConstantBuffer camera_data;

	VertexShader chunk_geometry_vs;
	PixelShader chunk_geometry_ps;
	InputLayout chunk_input_layout;
	SamplerState chunk_texture_sampler;

	std::unordered_map<std::string, ComPtr<ID3D11Texture2D>> textures;
	std::unordered_map<std::string, ComPtr<ID3D11RenderTargetView>> rtvs;
	std::unordered_map<std::string, ComPtr<ID3D11DepthStencilView>> dsvs;
	std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> srvs;

	bool	ping;
private:
	void	prepareTemporaryTextures();
};