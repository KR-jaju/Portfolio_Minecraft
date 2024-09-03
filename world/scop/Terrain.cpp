#include "pch.h"
#include "Terrain.h"
#include "Block.h"
#include "Graphics.h"
#include "TextureArray.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "Chunk.h"
#include <fstream>

#include <time.h>

Terrain::Terrain(
	int size_w, 
	int size_h, 
	HWND hwnd, 
	UINT width, 
	UINT height
)
{
	this->size_h = size_h;
	this->size_w = size_w;
	this->start_pos.x = -16 * (this->size_w / 2.f);
	this->start_pos.y = 16 * (this->size_h / 2.f);
	this->end_pos = -this->start_pos;
	this->graphic = make_shared<Graphics>(hwnd, width, height);
	//this->testFillChunk();
	this->createHeightMap();
	this->terrainsetVerticesAndIndices();
}

Terrain::~Terrain()
{
}

Index2 Terrain::getChunkIndex(float w_x, float w_z) const
{
	int x = floor(w_x);
	int t = x / 16;
	if (t * 16 > x)
		t -= 1;
	x = (t % size_w + size_w) % size_w;
	int z = floor(w_z);
	t = z / 16;
	if (t * 16 < z)
		t += 1;
	z = (t % size_h + size_h) % size_h;
	Index2 ans = Index2(x, z);
	if (this->chunks[ans.y][ans.x].get() != nullptr) {
		vec3 pos = this->chunks[ans.y][ans.x]->getChunkPos();
		pos += vec3(-0.0001, 0, 0.0001);
		float ez = pos.z - 16;
		float ex = pos.x + 16;
		if (w_x < pos.x || w_x > ex || w_z < ez || w_z > pos.z) {
			ans.flag = false;
		}
	}
	return ans;
}

void Terrain::fillChunk(vec2 chunk_pos)
{
	Index2 c_idx;
	c_idx = this->getChunkIndex(chunk_pos.x, chunk_pos.y);
	this->chunks[c_idx.y][c_idx.x] = make_shared<Chunk>();
	this->chunks[c_idx.y][c_idx.x]->setStartPos(chunk_pos.x, 0, chunk_pos.y);
	float x, z;
	float offset = 0.000001;
	for (int i = 0; i < 16; i++) {
		z = (chunk_pos.y - i + offset) / 32;
		for (int j = 0; j < 16; j++) {
			x = (chunk_pos.x + j + offset) / 32;
			double h_ = this->perlin_noise.getNoise2D(x, z, 3, 0.5);
			int16 h = static_cast<int16>((h_ + 1) * 0.5 * 30.f);
			this->chunks[c_idx.y][c_idx.x]->setHeight(j, i, h);
			for (int y = 0; y < h; y++) {
				this->chunks[c_idx.y][c_idx.x]->addBlock(j, y, i, 1);
			}
		}
	}
}

void Terrain::testFillChunk()
{
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			vec2 pos = this->start_pos + vec2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->getChunkIndex(pos.x, pos.y);
			this->chunks[c_idx.y][c_idx.x] = make_shared<Chunk>();
			this->chunks[c_idx.y][c_idx.x]->setStartPos(pos.x, 0, pos.y);
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					this->chunks[c_idx.y][c_idx.x]->setHeight(x, z, 30);
					for (int y = 0; y < 30; y++)
						this->chunks[c_idx.y][c_idx.x]->addBlock(x, y, z, 1);
				}
			}
		}
	}
}

void Terrain::createHeightMap()
{
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			vec2 pos = this->start_pos + vec2(j * 16, -i * 16);
			this->fillChunk(pos);
		}
	}
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
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			16,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"DIRECTION",
			0,
			DXGI_FORMAT_R32_SINT,
			0,
			24,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"CHUNKIDX",
			0,
			DXGI_FORMAT_R32G32_SINT,
			0,
			28,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	this->vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"TestVertexShader2.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout.data(),
		layout.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"TestPixelShader2.hlsl",
		"main",
		"ps_5_0"
	);

	this->graphic->getContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	this->graphic->getContext()->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	
	this->graphic->getContext()->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
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

void Terrain::Render
(
	Mat const& proj,
	Mat const& view,
	vec3 const& cam_pos
)
{
	CamPos cam;
	cam.pos = cam_pos;
	cam.r = 16.f * 1.f;
	MVP mvp;
	mvp.proj = proj.Transpose();
	mvp.view = view.Transpose();
	shared_ptr<ConstantBuffer> vertex_cbuffer;
	vertex_cbuffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		mvp
	);
	vertex_cbuffer->update(mvp);
	this->graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		vertex_cbuffer->getComPtr().GetAddressOf()
	);

	shared_ptr<ConstantBuffer> pixel_cbuffer;
	pixel_cbuffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		cam
	);
	pixel_cbuffer->update(cam);
	this->graphic->getContext()->PSSetConstantBuffers(
		0,
		1,
		pixel_cbuffer->getComPtr().GetAddressOf()
	);
	this->graphic->setClearColor(0.3, 0.3, 0.3, 1);
	this->graphic->renderBegin();
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			if (this->chunks[i][j]->getRenderFlag() == false)
				continue;
			this->chunks[i][j]->setVIBuffer(
				this->graphic,
				this->vertex_shader
			);
		}
	}
	this->graphic->renderEnd();
}

int Terrain::getBlockByIndex(
	Index3 const& b_idx, 
	Index2 const& c_idx
) const
{
	vec3 pos = this->chunks[c_idx.y][c_idx.x]->getChunkPos();
	vec3 adj;
	Index2 adj_cidx;
	if (b_idx.y > 255 || b_idx.y < 0)
		return 0;
	if (b_idx.x < -1 || b_idx.x > 16 || b_idx.z < -1 || b_idx.z > 16)
		assert(false);
	if (b_idx.x == -1) {
		adj = pos + vec3(-16, 0, 0);
		adj_cidx = this->getChunkIndex(adj.x, adj.z);
		if (adj_cidx.flag == false)
			return 0;
		return this->chunks[adj_cidx.y][adj_cidx.x]->getBlock(
			15, b_idx.y, b_idx.z);
	}
	if (b_idx.x == 16) {
		adj = pos + vec3(16, 0, 0);
		adj_cidx = this->getChunkIndex(adj.x, adj.z);
		if (adj_cidx.flag == false)
			return 0;
		return this->chunks[adj_cidx.y][adj_cidx.x]->getBlock(
			0, b_idx.y, b_idx.z);
	}
	if (b_idx.z == -1) {
		adj = pos + vec3(0, 0, 16);
		adj_cidx = this->getChunkIndex(adj.x, adj.z);
		if (adj_cidx.flag == false)
			return 0;
		return this->chunks[adj_cidx.y][adj_cidx.x]->
			getBlock(b_idx.x, b_idx.y, 15);
	}
	if (b_idx.z == 16) {
		adj = pos + vec3(0, 0, -16);
		adj_cidx = this->getChunkIndex(adj.x, adj.z);
		if (adj_cidx.flag == false)
			return 0;
		return this->chunks[adj_cidx.y][adj_cidx.x]->
			getBlock(b_idx.x, b_idx.y, 0);
	}
	return this->chunks[c_idx.y][c_idx.x]->getBlock(b_idx);
}

void Terrain::vertexAndIndexGenerator(
	vec2 const& pos,
	Index3 const& move,
	int dir,
	vector<VertexBlockUV>& vertices,
	vector<uint32>& indices
)
{
	Index2 c_idx = this->getChunkIndex(pos.x, pos.y);
	uint32 index = this->chunks[c_idx.y][c_idx.x]->getVerticesIdx();
	vec3 s_pos = this->chunks[c_idx.y][c_idx.x]->getStartPos();
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				int type = this->chunks[c_idx.y][c_idx.x]->getBlock(x, y, z);
				if (type == 0)
					continue;
				Index3 next(x + move.x, y + move.y, z + move.z);
				int tmp = this->getBlockByIndex(next, c_idx);
				if (tmp)
					continue;
				Block::addBlockFacePosAndTex(
					s_pos, 
					dir, 
					x, y, z, type,
					c_idx.y,
					c_idx.x,
					vertices);
				Block::addBlockFaceIndices(index, indices);
				index += 4;
			}
		}
	}
	this->chunks[c_idx.y][c_idx.x]->setVerticesIdx(index);
}

void Terrain::terrainsetVerticesAndIndices()
{
	Index3 move_arr[6] = {
		Index3(0, 1, 0),
		Index3(0, -1, 0),
		Index3(0, 0, 1),
		Index3(0, 0, -1),
		Index3(-1, 0, 0),
		Index3(1, 0, 0)
	};
	for (int i = 1; i < this->size_h - 1; i++) {
		for (int j = 1; j < this->size_w - 1; j++) {
			vec2 c_pos = this->start_pos + vec2(16 * j, -16 * i);
			Index2 c_idx = this->getChunkIndex(c_pos.x, c_pos.y);
			vector<VertexBlockUV> vertices;
			vector<uint32> indices;
			for (int dir = 0; dir < 6; dir++) {
				this->vertexAndIndexGenerator(
					c_pos,
					move_arr[dir],
					dir,
					vertices,
					indices
				);
			}
			this->chunks[c_idx.y][c_idx.x]->createVIBuffer(
				this->graphic,
				vertices,
				indices
			);
		}
	}
}

void Terrain::setSightChunk(int cnt)
{
	this->sight_r = cnt % min(this->size_h, this->size_w);
}

int Terrain::getBlock(float x, float y, float z) const
{
	WorldIndex ans = this->getBlockIndex(x, y, z);
	if (ans.flag == false)
		return 0;
	return this->chunks[ans.c_idx.y][ans.c_idx.x]->getBlock(ans.b_idx);
}

WorldIndex Terrain::getBlockIndex(float x, float y, float z) const
{
	WorldIndex ans;
	Index2 c_idx = this->getChunkIndex(x, z);
	vec3 pos = this->chunks[c_idx.y][c_idx.x]->getChunkPos();
	float ex = pos.x + 16.f;
	float ez = pos.z - 16.f;
	if (x < pos.x || x > ex || z > pos.z || z < ez || y < 0 || y > 255)
		return ans;
	ans.c_idx = c_idx;
	int ix = static_cast<int>(floor(x) - pos.x);
	int iy = static_cast<int>(floor(y));
	int iz = static_cast<int>(pos.z - floor(z));
	ans.b_idx = Index3(ix, iy, iz);
	ans.flag = true;
	return ans;
}


int Terrain::checkTerrainBoundary(float x, float z) const
{
	float r = 16.f * this->sight_r;
	int mask = 0;
	if (x - r < this->start_pos.x)
		mask |= 1 << 0; // left out
	if (x + r > this->end_pos.x)
		mask |= 1 << 1; // right out
	if (z + r > this->start_pos.y)
		mask |= 1 << 2; // back out
	if (z - r < this->end_pos.y)
		mask |= 1 << 3; // front out
	return mask;
}



/*
대각선 이동 후 대각선의 기저 벡터의 
*/