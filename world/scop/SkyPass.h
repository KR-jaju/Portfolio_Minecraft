#pragma once

#include "RenderPass.h"
#include "VertexShader.h"
#include "Cubemap.h"

class SkyPass : public RenderPass
{
public:
	SkyPass();
	void execute(RenderingContext& context, RenderGroup const& render_group);
private:
	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11RenderTargetView> hdr_output;
	ComPtr<ID3D11VertexShader>	skybox_vs;
	ComPtr<ID3D11PixelShader>	skybox_ps;
	ComPtr<ID3D11InputLayout>	skybox_il;
	std::vector<vec3> sphere_vertices;
	std::vector<uint32> sphere_indices;
	ComPtr<ID3D11Buffer> sphere_vertex_buffer;
	ComPtr<ID3D11Buffer> sphere_index_buffer;

	std::shared_ptr<Cubemap>	skybox_texture;

	void initialize(RenderingContext& context, AssetManager& asset_manager);
	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
	void initializeSkyboxShader(RenderingContext& context, std::wstring const& vs_path, std::wstring const& ps_path);
	void initializeSphereMesh(RenderingContext& context);
};
