#include "pch.h"
#include "Terrain.h"
#include "Block.h"
#include "Graphics.h"
#include "TextureArray.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "Chunk.h"
#include <fstream>

#include <time.h>

Terrain::Terrain(HWND hwnd, uint32 width, uint32 height)
{
	int dy[4] = { -1, 0, 1, 0 };
	int dx[4] = { 0, 1, 0, -1 };
	string file_name;

	float sx = -16.f * this->size_w * 0.5f;
	float sz = 16.f * this->size_h * 0.5f;
	this->start_pos = vec2(sx, sz);
	this->end_pos = vec2(sx + 16 * this->size_w, sz - 16 * this->size_h);
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++)
			this->terrain[i][j] = make_shared<Chunk>();
	}
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			this->terrain[i][j]->setStartPos(sx, 0, sz);
			file_name = "../chunk_files/";
			file_name += to_string(sx + 0.5) + "_";
			file_name += to_string(sz - 0.5) + ".txt";
			file_book.insert(file_name);
			for (int k = 0; k < 4; k++) {
				int ny = i + dy[k];
				int nx = j + dx[k];
				if (ny < 0 || ny >= this->size_h || nx < 0 || nx >= this->size_w)
					continue;
				if (dy[k] == -1) {
					this->terrain[i][j]->setChunk(this->
						terrain[ny][nx].get(), "back");
				}
				else if (dy[k] == 1) {
					this->terrain[i][j]->setChunk(this->
						terrain[ny][nx].get(), "front");
				}
				else if (dx[k] == -1) {
					this->terrain[i][j]->setChunk(this->
						terrain[ny][nx].get(), "left");
				}
				else if (dx[k] == 1) {
					this->terrain[i][j]->setChunk(this->
						terrain[ny][nx].get(), "right");
				}
			}
			sx += 16.f;
		}
		sx = -16.f * this->size_w * 0.5f;
		sz -= 16.f;
	}

	this->graphic = make_shared<Graphics>(
		hwnd,
		width,
		height
	);
	this->graphic->setClearColor(0.3f, 0.3f, 0.3f, 1.f);
	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(
		this->graphic->getDevice()
	);
	this->blend_state_arr.push_back(
		make_shared<BlendState>(this->graphic->getDevice())
	);

	// temp
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
}

Terrain::~Terrain()
{
}

void Terrain::setRender()
{
	// temp
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			if (this->terrain[i][j]->getBlockCnt() == 0)
				continue;
			this->terrain[i][j]->setRender(
				this->graphic,
				this->rasterizer_state,
				this->sampler_state,
				L"TestVertexShader2.hlsl",
				L"TestPixelShader2.hlsl",
				this->blend_state_arr[0]
			);
		}
	}
}

void Terrain::Render
(
	Mat const& proj,
	Mat const& view,
	vec3 const& cam_pos
)
{
	this->graphic->renderBegin();
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			if (this->terrain[i][j]->getBlockCnt() == 0)
				continue;
			// temp
			this->terrain[i][j]->setCamPos(cam_pos);
			// temp
			this->terrain[i][j]->Render(
				view, 
				proj,
				this->texture_array
			);
		}
	}
	this->graphic->renderEnd();
}

void Terrain::createHeightMap()
{
	float x, z;
	for (int i = 0; i < this->size_h * 16; i++) {
		int nz = i / 16;
		z = (this->start_pos.y - i) / 32;
		for (int j = 0; j < this->size_w * 16; j++) {
			int nx = j / 16;
			x = (j + this->start_pos.x) / 32;
			double res = this->perlin_noise.getNoise2D(x, z, 3, 0.5f);
			int16 h = static_cast<int16>(((res + 1.f) * 0.5f) * 30.f);
			this->terrain[nz][nx]->setHeight(j % 16, i % 16, h);
			for (int y = 0; y < h; y++)
				this->terrain[nz][nx]->setBlockInChunk(j % 16, y, i % 16, 1);
		}
	}
}

void Terrain::terrainsetVerticesAndIndices()
{
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			this->terrain[i][j]->setVerticesAndIndices();
			//this->terrain[i][j]->updateFile();
		}
	}
}

void Terrain::setSightChunk(int cnt)
{
	this->sight_r = cnt % min(this->size_h, this->size_w);
}

void Terrain::readTerrainForTest()
{
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++)
			this->terrain[i][j] = make_shared<Chunk>();
	}
	set<string>::iterator it = this->file_book.begin();
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			string str = *(it);
			set<string>::iterator itt = this->file_book.find(str);
			if (itt != this->file_book.end()) {
				this->terrain[i][j]->readFile(str);
			}
			it++;
		}
	}
}


WorldIndex Terrain::coordinateToIndex(
	float x, 
	float y, 
	float z
) const
{
	WorldIndex res = {};
	float end_x = this->start_pos.x + 16 * this->size_w;
	float end_y = this->start_pos.y - 16 * this->size_h;
	if (x < this->start_pos.x || x > end_x + 0.00001 ||
		y < end_y || y > this->start_pos.y + 0.00001) {
		res.w_idx = {-1, -1};
		return res;
	}
	res.w_idx.x = static_cast<int>(x - this->start_pos.x) / 16;
	res.w_idx.y = static_cast<int>(this->start_pos.y - z) / 16;
	
	vec3 start = this->terrain[res.w_idx.y][res.w_idx.x]->getStartPos();
	res.c_idx.x = static_cast<int>(x - start.x) % 16;
	res.c_idx.y = static_cast<int>(y) % 256;
	res.c_idx.z = static_cast<int>(start.z - z) % 16;
	return res;
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

int Terrain::relocateTerrain(float x, float z, int flag)
{
	if (flag & 1) {
		for (int i = 0; i < this->size_h; i++) {
			for (int j = this->size_w - 1; j > 0; j--)
				this->terrain[i][j] = this->terrain[i][j - 1];
		}
		for (int i = 0; i < this->size_h; i++)
			this->terrain[i][this->size_w - 1]->setChunk(nullptr, "right");
		this->start_pos = this->start_pos + vec2(-16, 0);
	}
	else if (flag & 2) {
		for (int i = 0; i < this->size_h; i++) {
			for (int j = 0; j < this->size_w - 1; j++)
				this->terrain[i][j] = this->terrain[i][j + 1];
		}
		for (int i = 0; i < this->size_h; i++)
			this->terrain[i][0]->setChunk(nullptr, "left");
		this->start_pos = this->start_pos + vec2(16, 0);
	}
	if (flag & 4) {
		for (int i = this->size_h - 1; i > 0; i--) {
			for (int j = 0; j < this->size_w; j++)
				this->terrain[i][j] = this->terrain[i - 1][j];
		}
		for (int i = 0; i < this->size_w; i++)
			this->terrain[this->size_h-1][i]->setChunk(nullptr, "front");
		this->start_pos = this->start_pos + vec2(0, 16);
	}
	else if (flag & 8) {
		for (int i = 0; i < this->size_h - 1; i++) {
			for (int j = 0; j < this->size_w; j++)
				this->terrain[i][j] = this->terrain[i + 1][j];
		}
		for (int i = 0; i < this->size_w; i++)
			this->terrain[0][i]->setChunk(nullptr, "back");
		this->start_pos = this->start_pos + vec2(0, -16);
	}
	this->end_pos.x = this->start_pos.x + 16.f * this->size_w;
	this->end_pos.y = this->start_pos.y - 16.f * this->size_h;
	return flag;
}

void Terrain::allocateTerrain(int flag)
{
	if (flag & 1)
		this->allocateTerrainLeft(flag);
	else if (flag & 2)
		this->allocateTerrainRiht(flag);
	if (flag & 4)
		this->allocateTerrainBack(flag);
	else if (flag & 8)
		this->allocateTerrainFront(flag);
}

void Terrain::allocateTerrainLeft(int flag)
{
	for (int i = 0; i < this->size_h; i++) {
		this->terrain[i][0] = make_shared<Chunk>();
		this->terrain[i][0]->setStartPos(this->start_pos.x, 0, 
			this->start_pos.y - 16 * i);
	}
	for (int i = 0; i < this->size_h; i++) {
		if (i)
			this->terrain[i][0]->setChunk(this->terrain[i - 1][0].get(),
				"back");
		if (i != this->size_h - 1)
			this->terrain[i][0]->setChunk(this->terrain[i + 1][0].get(),
				"front");
		this->terrain[i][0]->setChunk(this->terrain[i][1].get(), "right");
		this->terrain[i][1]->setChunk(this->terrain[i][0].get(), "left");
	}
	for (int i = 0; i < this->size_h * 16; i++) {
		int nz = i / 16;
		float z = (this->start_pos.y - i) / 32;
		for (int j = 0; j < 16; j++) {
			float x = (this->start_pos.x + j) / 32;
			double res = this->perlin_noise.getNoise2D(x, z, 3, 0.5);
			int16 h = static_cast<int16>(((res + 1.f) * 0.5f) * 30.f);
			this->terrain[nz][0]->setHeight(j, i % 16, h);
			for (int y = 0; y < h; y++)
				this->terrain[nz][0]->setBlockInChunk(j, y, i % 16, 1);
		}
	}
	for (int i = 0; i < this->size_h; i++) {
		this->terrain[i][0]->setVerticesAndIndices();
		this->terrain[i][0]->setRender(
			this->graphic,
			this->rasterizer_state,
			this->sampler_state,
			L"TestVertexShader2.hlsl",
			L"TestPixelShader2.hlsl",
			this->blend_state_arr[0]
		);
	}
}

void Terrain::allocateTerrainRiht(int flag)
{
	int ix = this->size_w - 1;
	float sx = this->start_pos.x + 16 * (this->size_w - 1);
	for (int i = 0; i < this->size_h; i++) {
		this->terrain[i][ix] = make_shared<Chunk>();
		this->terrain[i][ix]->setStartPos(sx, 0, this->start_pos.y - 16 * i);
	}
	for (int i = 0; i < this->size_h; i++) {
		if (i)
			this->terrain[i][ix]->setChunk(this->terrain[i - 1][ix].get(),
				"back");
		if (i != this->size_h - 1)
			this->terrain[i][ix]->setChunk(this->terrain[i + 1][ix].get(),
				"front");
		this->terrain[i][ix]->setChunk(this->terrain[i][ix - 1].get(),
			"left");
		this->terrain[i][ix - 1]->setChunk(this->terrain[i][ix].get(),
			"right");
	}
	for (int i = 0; i < this->size_h * 16; i++) {
		int nz = i / 16;
		float z = (this->start_pos.y - i) / 32;
		for (int j = 0; j < 16; j++) {
			float x = (sx + j) / 32;
			double res = this->perlin_noise.getNoise2D(x, z, 3, 0.5);
			int16 h = static_cast<int16>(((res + 1.f) * 0.5f) * 30.f);
			this->terrain[nz][ix]->setHeight(j, i % 16, h);
			for (int y = 0; y < h; y++)
				this->terrain[nz][ix]->setBlockInChunk(j, y, i % 16, 1);
		}
	}
	for (int i = 0; i < this->size_h; i++) {
		this->terrain[i][ix]->setVerticesAndIndices();
		this->terrain[i][ix]->setRender(
			this->graphic,
			this->rasterizer_state,
			this->sampler_state,
			L"TestVertexShader2.hlsl",
			L"TestPixelShader2.hlsl",
			this->blend_state_arr[0]
		);
	}
}

void Terrain::allocateTerrainBack(int flag)
{
	int s_idx = 0;
	int e_idx = this->size_w;
	if (flag & 1)
		s_idx += 1;
	else if (flag & 2)
		e_idx -= 1;
	for (int i = s_idx; i < e_idx; i++) {
		this->terrain[0][i] = make_shared<Chunk>();
		this->terrain[0][i]->setStartPos(this->start_pos.x + 16 * i, 0, 
			this->start_pos.y);
	}
	for (int i = s_idx; i < e_idx; i++) {
		if (i)
			this->terrain[0][i]->setChunk(this->terrain[0][i - 1].get(), "left");
		if (i != this->size_w - 1)
			this->terrain[0][i]->setChunk(this->terrain[0][i + 1].get(), "right");
		this->terrain[0][i]->setChunk(this->terrain[1][i].get(), "front");
		this->terrain[1][i]->setChunk(this->terrain[0][i].get(), "back");
	}
	for (int i = 0; i < 16; i++) {
		float sz = (this->start_pos.y - i) / 32;
		for (int j = 16 * s_idx; j < 16 * e_idx; j++) {
			int ix = j / 16;
			float sx = (this->start_pos.x + j) / 32;
			float res = this->perlin_noise.getNoise2D(sx, sz, 5, 0.5);
			int16 h = static_cast<int16>(((res + 1.f) * 0.5f) * 30.f);
			this->terrain[0][ix]->setHeight(j % 16, i, h);
			for (int y = 0; y < h; y++)
				this->terrain[0][ix]->setBlockInChunk(j % 16, y, i, 1);
		}
	}
	for (int i = s_idx; i < e_idx; i++) {
		this->terrain[0][i]->setVerticesAndIndices();
		this->terrain[0][i]->setRender(
			this->graphic,
			this->rasterizer_state,
			this->sampler_state,
			L"TestVertexShader2.hlsl",
			L"TestPixelShader2.hlsl",
			this->blend_state_arr[0]
		);
	}
}

void Terrain::allocateTerrainFront(int flag)
{
	int s_idx = 0;
	int e_idx = this->size_w;
	if (flag & 1)
		s_idx += 1;
	else if (flag & 2)
		e_idx -= 1;
	int iy = this->size_h - 1;
	for (int i = s_idx; i < e_idx; i++) {
		this->terrain[iy][i] = make_shared<Chunk>();
		this->terrain[iy][i]->setStartPos(
			this->start_pos.x + 16 * i, 
			0, this->start_pos.y - 16 * (this->size_h - 1));
	}
	for (int i = s_idx; i < e_idx; i++) {
		if (i)
			this->terrain[iy][i]->setChunk(this->terrain[iy][i - 1].get(), "left");
		if (i != this->size_w - 1)
			this->terrain[iy][i]->setChunk(this->terrain[iy][i + 1].get(), "right");
		this->terrain[iy][i]->setChunk(this->terrain[iy - 1][i].get(), "back");
		this->terrain[iy - 1][i]->setChunk(this->terrain[iy][i].get(), "front");
	}
	for (int i = 0; i < 16; i++) {
		float sz = (this->start_pos.y - i - 16 * (this->size_h - 1)) / 32;
		for (int j = 16 * s_idx; j < 16 * e_idx; j++) {
			int ix = j / 16;
			float sx = (this->start_pos.x + j) / 32;
			float res = this->perlin_noise.getNoise2D(sx, sz, 5, 0.5);
			int16 h = static_cast<int16>(((res + 1.f) * 0.5f) * 30.f);
			this->terrain[iy][ix]->setHeight(j % 16, i, h);
			for (int y = 0; y < h; y++)
				this->terrain[iy][ix]->setBlockInChunk(j % 16, y, i, 1);
		}
	}
	for (int i = s_idx; i < e_idx; i++) {
		this->terrain[iy][i]->setVerticesAndIndices();
		this->terrain[iy][i]->setRender(
			this->graphic,
			this->rasterizer_state,
			this->sampler_state,
			L"TestVertexShader2.hlsl",
			L"TestPixelShader2.hlsl",
			this->blend_state_arr[0]
		);
	}
}

void Terrain::terrainUpdate(float x, float z)
{
	int flag = checkTerrainBoundary(x, z);
	relocateTerrain(x, z, flag);
	allocateTerrain(flag);
}
