#include "pch.h"
#include "Terrain.h"
#include "Block.h"
#include "Graphics.h"
#include "TextureArray.h"
#include "RasterizerState.h"
#include "BlendState.h"

Terrain::Terrain(HWND hwnd, uint32 width, uint32 height)
{
	int dy[4] = { -1, 0, 1, 0 };
	int dx[4] = { 0, 1, 0, -1 };

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			for (int k = 0; k < 4; k++) {
				int ny = i + dy[k];
				int nx = j + dx[k];
				if (ny < 0 || ny >= 30 || nx < 0 || nx >= 30)
					continue;
				if (dy[k] == -1) {
					this->terrain[i][j].setBack(&this->terrain[ny][nx]);
				}
				else if (dy[k] == 1) {
					this->terrain[i][j].setFront(&this->terrain[ny][nx]);
				}
				else if (dx[k] == -1) {
					this->terrain[i][j].setLeft(&this->terrain[ny][nx]);
				}
				else if (dx[k] == 1) {
					this->terrain[i][j].setRight(&this->terrain[ny][nx]);
				}
			}
		}
	}
	this->graphic = make_shared<Graphics>(
		hwnd,
		width,
		height
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
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

void Terrain::Render()
{
	this->graphic->renderBegin();
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			if (this->terrain[i][j].getBlockCnt() == 0)
				continue;
			this->terrain[i][j].Render(this->view, this->proj);
		}
	}
	this->graphic->renderEnd();
}

void Terrain::createMap()
{
}
