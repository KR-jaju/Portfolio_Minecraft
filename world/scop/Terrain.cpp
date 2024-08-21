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
		path_arr
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
	this->sight_r = cnt;
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
		mask |= 1 << 0;
	if (x + r > this->end_pos.x)
		mask |= 1 << 1;
	if (z + r > this->start_pos.y)
		mask |= 1 << 2;
	if (z - r < this->end_pos.y)
		mask |= 1 << 3;
	return mask;
}

int Terrain::relocateTerrain(float x, float z, int flag)
{
	if (flag & 1) {
		for (int i = 0; i < this->size_h; i++) {
			for (int j = this->size_w - 1; j > 0; j--)
				this->terrain[i][j] = this->terrain[i][j - 1];
		}
	}
	else if (flag & 2) {
		
	}
	if (flag & 4) {
		
	}
	else if (flag & 8) {
		
	}
	return flag;
}

void Terrain::allocateTerrain(int flag)
{
	if (flag & 1) {
		vec3 pos;
		if (flag & 4)
			pos = this->terrain[1][1]->getStartPos() + vec3(-16, 0, 16);
		else
			pos = this->terrain[0][1]->getStartPos() + vec3(-16, 0, 0);
		for (int i = 0; i < this->size_h; i++) {
			this->terrain[i][0] = make_shared<Chunk>();
			this->terrain[i][0]->setStartPos(pos.x, 0, pos.z);
			if (i)
				this->terrain[i][0]->setChunk(this->terrain[i - 1][0].get(), 
					"back");
			if (i != this->size_h - 1)
				this->terrain[i][0]->setChunk(this->terrain[i + 1][0].get(),
					"front");
			this->terrain[i][0]->setChunk(nullptr, "left");
			this->terrain[i][0]->setChunk(this->terrain[i][1].get(), "right");
		}
		for (int i = 0; i < this->size_h * 16; i++) {
			int nz = i / 16;
			float z = (this->start_pos.y - i) / 32;
			double res = this->perlin_noise.getNoise2D(
				this->start_pos.x / 32, z, 3, 0.5);
			int16 h = static_cast<int16>(((res + 1.f) * 0.5f) * 30.f);
			this->terrain[nz][0]->setHeight(0, i % 16, h);
			for (int y = 0; y < h; y++)
				this->terrain[nz][0]->setBlockInChunk(0, y, i % 16, 1);
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
	else if (flag & 2) {
		
	}
	if (flag & 4) {

	}
	else if (flag & 8) {

	}
}

void Terrain::terrainUpdate(float x, float z)
{
	int flag = checkTerrainBoundary(x, z);
	relocateTerrain(x, z, flag);
	allocateTerrain(flag);
}
