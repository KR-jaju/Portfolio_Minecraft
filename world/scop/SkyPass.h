#pragma once

#include "RenderPass.h"
#include "VertexShader.h"
#include "Cubemap.h"

class SkyPass : public RenderPass
{
public:
	SkyPass(Renderer& renderer, RenderingContext& context);
	void execute(RenderingContext& context);
private:
	ComPtr<ID3D11DepthStencilView> dsv;

	ComPtr<ID3D11RenderTargetView> hdr_output;

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
};
