#include "pch.h"
#include "Terrain.h"
#include "Block.h"
#include "Graphics.h"
#include "TextureArray.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"

Terrain::Terrain(HWND hwnd, uint32 width, uint32 height)
{
	int dy[4] = { -1, 0, 1, 0 };
	int dx[4] = { 0, 1, 0, -1 };

	float sx = -240.f;
	float sz = 234.f;
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++)
			this->terrain[i][j] = make_shared<Chunk>();
	}
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			this->terrain[i][j]->setStartPos(sx, 0, sz);
			for (int k = 0; k < 4; k++) {
				int ny = i + dy[k];
				int nx = j + dx[k];
				if (ny < 0 || ny >= 30 || nx < 0 || nx >= 30)
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
		sz -= 16.f;
	}
	// test
	{
		this->terrain[0][0]->setStartPos(0, 0, 0);
	}
	this->graphic = make_shared<Graphics>(
		hwnd,
		width,
		height
	);
	this->graphic->setClearColor(0.3, 0.3, 0.3, 1.f);
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
	/*for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			this->terrain[i][j]->setRender(
				this->graphic,
				this->rasterizer_state,
				this->sampler_state,
				L"WorldVertexShader.hlsl",
				L"WorldPixelShader.hlsl",
				this->blend_state_arr[0]
			);
		}
	}*/
	this->terrain[0][0]->setRender(
		this->graphic,
		this->rasterizer_state,
		this->sampler_state,
		L"WorldVertexShader.hlsl",
		L"WorldPixelShader.hlsl",
		this->blend_state_arr[0]
	);
}

void Terrain::Render()
{
	// test
	this->graphic->renderBegin();
	/*for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			if (this->terrain[i][j]->getBlockCnt() == 0)
				continue;
			this->terrain[i][j]->Render(this->view, this->proj);
		}
	}*/
	this->terrain[0][0]->Render(
		this->view, 
		this->proj, 
		this->texture_array
	);
	this->graphic->renderEnd();
}

void Terrain::setCam(Mat view, Mat proj)
{
	this->view = view;
	this->proj = proj;
}

void Terrain::createHeightMap()
{
	int idx_y = 0;
	int idx_x = 0;
	/*for (int i = 0; i < 480; i++) {
		for (int j = 0; j < 480; j++) {
			float res = this->perlin_noise.getNoise2D(
				i / 4.f + 0.1, j / 4.f + 0.1, 5, 0.5);
			res = ((res + 1.f) * 0.5f) * 255;
			this->height_map[i][j] = static_cast<int16>(res);
			this->terrain[idx_y][idx_x]->setBlockInChunk(
				j % 16, res, i % 16, 1);
			if (j == 15)
				idx_x += 1;
		}
		idx_x = 0;
		if (i == 15)
			idx_y += 1;
	}*/
	// test
	{
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				float res = this->perlin_noise.getNoise2D(
					i / 1.0005f, j / 1.0005f, 5, 0.5f);
				res = ((res + 1.f) * 0.5f) * 30.f;
				this->height_map[i][j] = static_cast<int16>(res);
				for (int k = 0; k < this->height_map[i][j]; k++) {
					this->terrain[0][0]->setBlockInChunk(j % 16, k, i % 16, 1);
				}
			}
		}
		//this->terrain[0][0]->showChunk();
		//this->terrain[0][0]->setBlockInChunk(0, 0, 0, 1);
	}
	/*{
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++) {
				cout << this->height_map[i][j] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}*/
}

void Terrain::terrainsetVerticesAndIndices()
{
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			this->terrain[i][j]->setVerticesAndIndices();
		}
	}
	//this->terrain[0][0]->setVerticesAndIndices();
	//this->terrain[0][0]->setStartPos()
}
