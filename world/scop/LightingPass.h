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
	ComPtr<ID3D11DepthStencilView> dsv;

	ComPtr<ID3D11RenderTargetView> hdr_output;

	VertexShader	lighting_vs;
	PixelShader	lighting_ps;
	SamplerState copy_sampler;

	VertexShader	skybox_vs;
	PixelShader		skybox_ps;
	InputLayout		skybox_il;
	std::vector<vec3> sphere_vertices;
	std::vector<uint32> sphere_indices;
	Buffer<vec3> sphere_vertex_buffer;
	Buffer<uint32> sphere_index_buffer;

	Cubemap	skybox_texture;

	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
	void	drawSkybox(RenderingContext& context);
};
