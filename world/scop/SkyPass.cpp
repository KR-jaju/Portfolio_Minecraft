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

SkyPass::SkyPass() {}

void SkyPass::execute(RenderingContext& context, RenderGroup const& render_group)
{
	ID3D11DeviceContext* const dc = context.graphics.getContext().Get();
	uint32 stride = sizeof(vec3);
	uint32 offset = 0;

	this->bind(context);
	dc->IASetInputLayout(this->skybox_il.Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->IASetVertexBuffers(0, 1, this->sphere_vertex_buffer.GetAddressOf(), &stride, &offset);
	dc->IASetIndexBuffer(this->sphere_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	dc->VSSetShader(this->skybox_vs.Get(), nullptr, 0);
	dc->PSSetShader(this->skybox_ps.Get(), nullptr, 0);
	dc->PSSetSamplers(0, 1, context.chunk_texture_sampler.getComPtr().GetAddressOf());
	dc->PSSetShaderResources(0, 1, this->skybox_texture->getComPtr().GetAddressOf());
	dc->DrawIndexed(this->sphere_indices.size(), 0, 0);
	this->unbind(context);
}


void SkyPass::initialize(RenderingContext& context, AssetManager& asset_manager)
{
	this->dsv = context.dsvs["gbuffer_depth"];
	this->hdr_output = context.ping ? context.rtvs["hdr_temporary[0]"] : context.rtvs["hdr_temporary[1]"];
	this->skybox_texture = asset_manager.load<Cubemap>(L"./textures/skybox/HDRI/MyCubesEnvHDR.dds");
	this->initializeSkyboxShader(context, L"SkyPassVS.hlsl", L"SkyPassPS.hlsl");
	this->initializeSphereMesh(context);
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

void SkyPass::initializeSkyboxShader(RenderingContext& context, std::wstring const& vs_path, std::wstring const& ps_path)
{
	uint32 const compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	ComPtr<ID3DBlob> shader_blob;

	HRESULT hr = D3DCompileFromFile(vs_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreateVertexShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->skybox_vs.GetAddressOf());
	CHECK(hr);
	hr = device->CreateInputLayout(InputLayouts::layout_skybox, 1, shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), this->skybox_il.GetAddressOf());
	CHECK(hr);
	hr = D3DCompileFromFile(ps_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreatePixelShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->skybox_ps.GetAddressOf());
	CHECK(hr);
}


void SkyPass::initializeSphereMesh(RenderingContext& context)
{
	ComPtr<ID3D11Device> device = context.graphics.getDevice();
	D3D11_BUFFER_DESC desc = {};
	D3D11_SUBRESOURCE_DATA data = {};

	this->sphere_vertices = std::move(createSphereVertices(3));
	this->sphere_indices = std::move(createSphereIndices(3));
	desc.ByteWidth = sizeof(vec3) * this->sphere_vertices.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = this->sphere_vertices.data();
	HRESULT hr = device->CreateBuffer(&desc, &data, this->sphere_vertex_buffer.GetAddressOf());
	CHECK(hr);
	desc.ByteWidth = sizeof(uint32) * this->sphere_indices.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	data.pSysMem = this->sphere_indices.data();
	hr = device->CreateBuffer(&desc, &data, this->sphere_index_buffer.GetAddressOf());
	CHECK(hr);
}
