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


Terrain::Terrain(int size_w, int size_h)
{
	this->size_h = size_h;
	this->size_w = size_w;
	this->start_pos.x = -16 * (this->size_w / 2);
	this->start_pos.y = 16 * (this->size_h / 2);
	this->end_pos = -this->start_pos;
	this->terrain = new int[16 * 16 * 256 * this->size_w * this->size_h];
	this->height_map = new int[16 * 16 * this->size_w * this->size_h];
}

Terrain::~Terrain()
{
	delete[] this->terrain;
	delete[] this->height_map;
}

pair<int, int> Terrain::getIndexXZ(float w_x, float w_z)
{
	int x = floor(w_x);
	int sx = static_cast<int>(this->start_pos.x);
	int d = (x - sx) / 16;
	x = (d % this->size_w + this->size_w) % this->size_w;
	int z = floor(w_z);
	int sz = static_cast<int>(this->start_pos.y);
	d = (sz - z) / 16;
	z = (d % this->size_h + this->size_h) % this->size_h;
	return pair<int, int>(x, z);
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

void Terrain::setGraphics(HWND hwnd, uint32 width, uint32 height)
{
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