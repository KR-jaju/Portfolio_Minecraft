#include "pch.h"
#include "Terrain.h"
#include "Block.h"
#include "Chunk.h"
#include "thread"

#include "Graphics.h"
#include "TextureArray.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "Buffer.h"

#include <time.h>

#define STDZERO 0.00000001

bool inChunkBoundary(int x, int y, int z) {
	if (x >= 0 && x < 16 && z >= 0 && z < 16 && y >= 0 && y < 256)
		return true;
	return false;
}

vec3 intersectionRayAndPlane(
	vec3 const& r_pos,
	vec3 const& r_dir,
	vec3 const& p_pos,
	vec3 const& p_dir
)
{
	vec3 res;
	float t;
	t = (p_pos.Dot(p_dir) - r_pos.Dot(p_dir)) / p_dir.Dot(r_dir);
	res = r_pos + t * r_dir;
	return res;
}

Terrain::Terrain(
	int size_w,
	int size_h,
	HWND hwnd,
	UINT width,
	UINT height,
	int fov_chunk,
	int thread_cnt
)
{
	this->graphic = make_shared<Graphics>(hwnd, width, height);
	this->m_manager = make_shared<Map>(
		size_w,
		size_h,
		fov_chunk,
		thread_cnt,
		this->graphic
	);
}

Terrain::~Terrain()
{
}

void Terrain::setRender()
{
	vector<wstring> path_arr = {
		L"grass_top.png",
		L"grass_bottom.png",
		L"grass_side.png"
	};
	this->texture_array = make_shared<TextureArray>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		path_arr,
		0
	);
	this->blend_state_arr.push_back(
		make_shared<BlendState>(this->graphic->getDevice())
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->depth_rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_FRONT
	);
	this->sampler_state = make_shared<SamplerState>(this->graphic->getDevice());
	vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{
			"TYPE",
			0,
			DXGI_FORMAT_R32_SINT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			4,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			16,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			28,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"DIRECTION",
			0,
			DXGI_FORMAT_R32_SINT,
			0,
			36,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"SHADOW",
			0,
			DXGI_FORMAT_R32_SINT,
			0,
			40,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	this->vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"TestVertexShader.hlsl",
		"main",
		"vs_5_0"
	);
	this->depth_vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"DepthVertexShader.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout.data(),
		layout.size(),
		this->vertex_shader->getBlob()
	);
	this->depth_input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout.data(),
		layout.size(),
		this->depth_vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"TestPixelShader.hlsl",
		"main",
		"ps_5_0"
	);
	this->depth_pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"DepthPixelShader.hlsl",
		"main",
		"ps_5_0"
	);
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.BorderColor[0] = 0.f;
	comparisonSamplerDesc.BorderColor[1] = 0.f;
	comparisonSamplerDesc.BorderColor[2] = 0.f;
	comparisonSamplerDesc.BorderColor[3] = 0.f;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	comparisonSamplerDesc.MinLOD = 0;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	this->depth_sampler_state = make_shared<SamplerState>(
		this->graphic->getDevice(),
		comparisonSamplerDesc
	);
}

void Terrain::setRenderPipeLine(int flag)
{
	this->graphic->getContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	if (flag == 0) {
		this->graphic->getContext()->IASetInputLayout(
			this->input_layout->getComPtr().Get()
		);
		this->graphic->getContext()->VSSetShader(
			this->vertex_shader->getComPtr().Get(),
			nullptr,
			0
		);
		this->graphic->getContext()->RSSetState(
			this->rasterizer_state->getComPtr().Get()
		);
		this->graphic->getContext()->PSSetShader(
			this->pixel_shader->getComPtr().Get(),
			nullptr,
			0
		);
		this->graphic->getContext()->PSSetSamplers(
			0,
			1,
			this->sampler_state->getComPtr().GetAddressOf()
		);
		this->graphic->getContext()->PSSetSamplers(
			1,
			1,
			this->depth_sampler_state->getComPtr().GetAddressOf()
		);
		this->graphic->getContext()->PSSetShaderResources(
			0,
			1,
			this->texture_array->getComPtr().GetAddressOf()
		);

		this->graphic->getContext()->OMSetBlendState(
			this->blend_state_arr[0]->getComPtr().Get(),
			this->blend_state_arr[0]->getBlendFactor(),
			this->blend_state_arr[0]->getSampleMask()
		);
	}
	else {
		this->graphic->getContext()->IASetInputLayout(
			this->depth_input_layout->getComPtr().Get()
		);

		this->graphic->getContext()->VSSetShader(
			this->depth_vertex_shader->getComPtr().Get(),
			nullptr,
			0
		);
		this->graphic->getContext()->RSSetState(
			this->rasterizer_state->getComPtr().Get()
		);
		this->graphic->getContext()->PSSetShader(
			this->depth_pixel_shader->getComPtr().Get(),
			nullptr,
			0
		);
	}
}

void Terrain::DepthRender()
{
	this->setRenderPipeLine(1);
	D3D11_VIEWPORT view_port;
	ZeroMemory(&view_port, sizeof(view_port));
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	int times = 10;
	view_port.Width = pow(2, times);
	view_port.Height = pow(2, times);
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	this->graphic->getContext()->RSSetViewports(1, &(view_port));

	for (int i = 0; i < this->m_manager->m_info.size_h; i++) {
		for (int j = 0; j < this->m_manager->m_info.size_w; j++) {
			if (this->m_manager->m_info.chunks[i][j]->render_flag == false) {
				continue;
			}
			this->m_manager->m_info.chunks[i][j]->setDepthRender(
				this->graphic,
				this->depth_vertex_shader
			);
		}
	}
}

void Terrain::Render
(
	Mat const& cam_view,
	Mat const& cam_proj,
	vec3 const& cam_pos
)
{
	this->graphic->renderBegin();
	this->setRenderPipeLine(0);
	CamPos cam;
	cam.pos = cam_pos;
	cam.r = 16.f * 1.f;
	MVP mvp;
	mvp.proj = cam_proj.Transpose();
	mvp.view = cam_view.Transpose();
	ConstantBuffer vertex_cbuffer(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		mvp
	);
	this->graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		vertex_cbuffer.getComPtr().GetAddressOf()
	);
	ConstantBuffer pixel_cbuffer(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		cam
	);
	this->graphic->getContext()->PSSetConstantBuffers(
		0,
		1,
		pixel_cbuffer.getComPtr().GetAddressOf()
	);
	this->graphic->setClearColor(0.3, 0.3, 0.3, 1);
	for (int i = 0; i < this->m_manager->m_info.size_h; i++) {
		for (int j = 0; j < this->m_manager->m_info.size_w; j++) {
			if (this->m_manager->m_info.chunks[i][j]->render_flag == false) {
				continue;
			}
			this->m_manager->m_info.chunks[i][j]->setRender(
				this->graphic,
				this->vertex_shader,
				0
			);
		}
	}
	this->graphic->renderEnd();
}

void Terrain::putBlock(
	vec3 const& ray_pos,
	vec3 const& ray_dir,
	int type
)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		WorldIndex add_idx;
		Index2& cidx = widx.c_idx;
		Index3& bidx = widx.b_idx;
		vec3 const& pos = widx.pos;
		int dir_flag = -1;
		if (widx.dir == 0) {
			if (ray_pos.y > pos.y && pos.y + 1 < 256) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y + 1, pos.z);
				dir_flag = 0;
			}
			else if (ray_pos.y < pos.y && pos.y - 1 > -1) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y - 1, pos.z);
				dir_flag = 1;
			}
		}
		else if (widx.dir == 1) {
			if (ray_pos.z < pos.z) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z - 1);
				dir_flag = 2;
			}
			else {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z + 1);
				dir_flag = 3;
			}
		}
		else {
			if (ray_pos.x < pos.x) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x - 1, pos.y, pos.z);
				dir_flag = 4;
			}
			else {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x + 1, pos.y, pos.z);
				dir_flag = 5;
			}
		}
		if (add_idx.flag) {
			vector<Index2> v_idx;
			cidx = add_idx.c_idx;
			bidx = add_idx.b_idx;
			Index2 adj_idx;
			Index2 const& cpos = this->m_manager->m_info.chunks[cidx.y][cidx.x]->chunk_pos;
			if (bidx.x == 0) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x - 16, cpos.y);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 15, bidx.y, bidx.z)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.x == 15) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x + 16, cpos.y);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 0, bidx.y, bidx.z)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.z == 15) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y - 16);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 0)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.z == 0) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y + 16);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 15)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			this->m_manager->m_info.addBlock(cidx, bidx, type);
			Index3 bidx;
			for (int y = bidx.y; y >= 0; y--) {
				this->m_manager->m_info.setLight(cidx, bidx.x, y, bidx.z, 0);
			}
			this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
			int16& max_h = this->m_manager->m_info.chunks[cidx.y][cidx.x]->max_h;
			max_h = max(max_h, bidx.y + 1);
			v_idx.push_back(cidx);
			this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
		}
	}
}

void Terrain::deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		this->m_manager->m_info.addBlock(widx.c_idx, widx.b_idx, 0);
		this->m_manager->l_system.lightPropagationGather(widx.c_idx, widx.b_idx);
		
		this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->vertices_idx = 0;
		vector<Index2> v_idx;
		v_idx.push_back(widx.c_idx);
		Index2 cidx;
		Index2 const& pos = this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->chunk_pos;
		if (widx.b_idx.x == 0) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x - 16, pos.y);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.x == 15) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x + 16, pos.y);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.z == 0) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y + 16);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.z == 15) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y - 16);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
	}
}

pair<int, int> Terrain::getBlock(float x, float y, float z)
{
	WorldIndex widx = this->m_manager->m_info.getBlockIndex(x, y, z);
	pair<int, int> block_info(0, 0);
	if (widx.flag) {
		block_info.first = this->m_manager->m_info.findBlock(widx.c_idx, widx.b_idx);
		block_info.second = this->m_manager->m_info.findLight(
			widx.c_idx,
			widx.b_idx.x,
			widx.b_idx.y,
			widx.b_idx.z
		);
	}
	return block_info;
}

void Terrain::setSightChunk(int cnt)
{
	int max_fov = min(this->m_manager->m_info.size_h - 1, 
		this->m_manager->m_info.size_w - 1);
	this->m_manager->setSightChunk(min(max_fov, cnt));
}

void Terrain::userPositionCheck(float x, float z)
{
	this->m_manager->userPositionCheck(x, z);
}

int16 Terrain::getHeight(float x, float z) const
{
	WorldIndex w_idx = m_manager->m_info.getBlockIndex(x, 0, z);
	return m_manager->m_info.findHeight(w_idx.c_idx, w_idx.b_idx);
}
