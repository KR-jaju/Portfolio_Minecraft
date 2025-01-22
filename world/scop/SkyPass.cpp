#include "pch.h"
#include "SkyPass.h"
#include <iostream>
#include "InputLayouts.h"

static std::vector<vec3>	createSphereVertices(int subdivision)
{
	std::vector<vec3> vertices;

	for (int v = 0; v <= subdivision; ++v)
	{
		for (int u = 0; u <= subdivision; ++u)
		{
			vertices.emplace_back(1, 1 - 2 * (float)v / subdivision, 1 - 2 * (float)u / subdivision); // east
			vertices.back().Normalize();
			vertices.emplace_back(-1, 1 - 2 * (float)v / subdivision, -1 + 2 * (float)u / subdivision); // west
			vertices.back().Normalize();
			vertices.emplace_back(1 - 2 * (float)u / subdivision, 1, 1 - 2 * (float)v / subdivision); // up
			vertices.back().Normalize();
			vertices.emplace_back(1 - 2 * (float)u / subdivision, -1, -1 + 2 * (float)v / subdivision); // down
			vertices.back().Normalize();
			vertices.emplace_back(-1 + 2 * (float)u / subdivision, 1 - 2 * (float)v / subdivision, 1); // north
			vertices.back().Normalize();
			vertices.emplace_back(1 - 2 * (float)u / subdivision, 1 - 2 * (float)v / subdivision, -1); // south
			vertices.back().Normalize();
		}
	}
	return (vertices);
}
static std::vector<uint32>	createSphereIndices(int subdivision)
{
	std::vector<uint32> indices;

	for (int v = 0; v < subdivision; ++v)
	{
		for (int u = 0; u < subdivision; ++u)
		{
			for (int offset = 0; offset < 6; ++offset)
			{
				indices.emplace_back((u * (subdivision + 1) + v) * 6 + offset);
				indices.emplace_back(((u + 1) * (subdivision + 1) + v) * 6 + offset);
				indices.emplace_back((u * (subdivision + 1) + (v + 1)) * 6 + offset);

				indices.emplace_back(((u + 1) * (subdivision + 1) + v) * 6 + offset);
				indices.emplace_back(((u + 1) * (subdivision + 1) + (v + 1)) * 6 + offset);
				indices.emplace_back((u * (subdivision + 1) + (v + 1)) * 6 + offset);
			}
		}
	}
	return (indices);
}

SkyPass::SkyPass(Renderer& renderer, RenderingContext& context)
	: dsv(context.dsvs["gbuffer_depth"]),
	hdr_output(context.ping ? context.rtvs["hdr_temporary[0]"] : context.rtvs["hdr_temporary[1]"]),
	skybox_vs(context.graphics.getDevice(), L"SkyPassVS.hlsl", "main", "vs_5_0"),
	skybox_ps(context.graphics.getDevice(), L"SkyPassPS.hlsl", "main", "ps_5_0"),
	skybox_il(context.graphics.getDevice(), InputLayouts::layout_skybox, 1, this->skybox_vs.getBlob()),
	sphere_vertices(createSphereVertices(3)),
	sphere_indices(createSphereIndices(3)),
	sphere_vertex_buffer(context.graphics.getDevice(), this->sphere_vertices.data(), this->sphere_vertices.size(), D3D11_BIND_VERTEX_BUFFER),
	sphere_index_buffer(context.graphics.getDevice(), this->sphere_indices.data(), this->sphere_indices.size(), D3D11_BIND_INDEX_BUFFER),
	skybox_texture(context.graphics.getDevice(), L"./textures/skybox/HDRI/MyCubesEnvHDR.dds")
{
}

void SkyPass::execute(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();
	uint32 stride = this->sphere_vertex_buffer.getStride();
	uint32 offset = this->sphere_vertex_buffer.getOffset();

	this->bind(context);
	dc->IASetInputLayout(this->skybox_il.getComPtr().Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->IASetVertexBuffers(0, 1, this->sphere_vertex_buffer.getComPtr().GetAddressOf(), &stride, &offset);
	dc->IASetIndexBuffer(this->sphere_index_buffer.getComPtr().Get(), DXGI_FORMAT_R32_SINT, 0);
	dc->VSSetShader(this->skybox_vs.getComPtr().Get(), nullptr, 0);
	dc->PSSetShader(this->skybox_ps.getComPtr().Get(), nullptr, 0);
	dc->VSSetConstantBuffers(0, 1, context.camera_data.getComPtr().GetAddressOf());
	dc->PSSetSamplers(0, 1, context.chunk_texture_sampler.getComPtr().GetAddressOf());
	dc->PSSetShaderResources(0, 1, this->skybox_texture.getComPtr().GetAddressOf());
	dc->DrawIndexed(this->sphere_indices.size(), 0, 0);
	this->unbind(context);
}

void SkyPass::bind(RenderingContext& context)
{
	D3D11_VIEWPORT const viewport = {
		0, 0, context.viewport_width, context.viewport_height, 0.0f, 1.0f
	};
	ID3D11DeviceContext* dc = context.graphics.getContext().Get();
	ID3D11RenderTargetView* const views[3] = {
		this->hdr_output.Get()
	};
	float const albedo_clear[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	dc->OMSetRenderTargets(1, views, this->dsv.Get());
	dc->RSSetState(context.rasterizer_state.getComPtr().Get());
	dc->RSSetViewports(1, &viewport);
}
void SkyPass::unbind(RenderingContext& context)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();

	dc->OMSetRenderTargets(0, nullptr, nullptr);

	dc->PSSetShader(nullptr, nullptr, 0);

	dc->VSSetShader(nullptr, nullptr, 0);

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
}

