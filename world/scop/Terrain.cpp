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


Terrain::Terrain(HWND hwnd, uint32 width, uint32 height)
{
	int dy[4] = { -1, 0, 1, 0 };
	int dx[4] = { 0, 1, 0, -1 };

	float sx = -16.f * this->size_w * 0.5f;
	float sz = 16.f * this->size_h * 0.5f;
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++)
			this->terrain[i][j] = make_shared<Chunk>();
	}
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			this->terrain[i][j]->setStartPos(sx, 0, sz);
			for (int k = 0; k < 4; k++) {
				int ny = i + dy[k];
				int nx = j + dx[k];
				if (ny < 0 || ny >= this->size_h || nx < 0 || nx >= this->size_w)
					continue;
				if (dy[k] == -1) {
					this->terrain[i][j]->setBack(this->terrain[ny][nx].get());
				}
				else if (dy[k] == 1) {
					this->terrain[i][j]->setFront(this->terrain[ny][nx].get());
				}
				else if (dx[k] == -1) {
					this->terrain[i][j]->setLeft(this->terrain[ny][nx].get());
				}
				else if (dx[k] == 1) {
					this->terrain[i][j]->setRight(this->terrain[ny][nx].get());
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

void Terrain::setViewAndProj
(
	Mat const& view, 
	Mat const& proj
)
{
	this->view = view;
	this->proj = proj;
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
				L"WorldVertexShader.hlsl",
				L"WorldPixelShader.hlsl",
				this->blend_state_arr[0]
			);
		}
	}
}

void Terrain::Render()
{
	this->graphic->renderBegin();
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			if (this->terrain[i][j]->getBlockCnt() == 0)
				continue;
			this->terrain[i][j]->Render(
				this->view, 
				this->proj,
				this->texture_array
			);
		}
	}
	this->graphic->renderEnd();
}

void Terrain::setCam(Mat view, Mat proj)
{
	this->view = view;
	this->proj = proj;
}

void Terrain::createHeightMap()
{
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					double res = this->perlin_noise.getNoise2D(
						(x + 16 * j) / 100.f,
						(z + 16 * i) / 100.f,
						5,
						1.2f
					);
					res = ((res + 1.f) * 0.5f) * 30.f;
					this->height_map[i][j] = static_cast<int16>(res);
					for (int y = 0; y < this->height_map[i][j]; y++) {
						this->terrain[i][j]->setBlockInChunk(
							x, y, z, 1);
					}
				}
			}
		}
	}
}

void Terrain::terrainsetVerticesAndIndices()
{
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			this->terrain[i][j]->setVerticesAndIndices();
		}
	}
	cout << "end vertices and indices" << endl;
}

vector<pair<int, int>> Terrain::coordinateToIndex(
	float x, 
	float y, 
	float z
) const
{
	vector<pair<int, int>> res;
	int idx_x;
	int idx_xx;
	int idx_y;
	int idx_z;
	int idx_zz;
	vec3 start = this->terrain[0][0]->getStartPos();
	vec3 end = this->terrain[this->size_h - 1][this->size_w - 1]->
		getStartPos() + vec3(16.f, 0.f, -16.f);
	if (x < start.x || x > end.x || z > start.z || z < end.z || y < 0 || y > 255)
		return res;
	x -= start.x;
	x = abs(x);
	idx_x = static_cast<int>(x) / 16;
	idx_xx = static_cast<int>(x) % 16;
	res.push_back({ idx_x, idx_xx });
	idx_y = static_cast<int>(y) % 256;
	res.push_back({ idx_y, -1 });
	idx_z = static_cast<int>(z) / 16;
	idx_zz = static_cast<int>(z) % 16;
	res.push_back({ idx_z, idx_zz });
	return res;
}
