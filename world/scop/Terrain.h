#pragma once

#include "Chunk.h"

class Graphics;
class RasterizerState;
class TextureArray;
class BlendState;

class Terrain
{
public:
	Terrain(HWND hwnd, uint32 width, uint32 height);
	~Terrain();
	void setViewAndProj(Mat const& view, Mat const& proj);
	void Render();
	void createMap();
private:
	Mat view;
	Mat proj;
	map<vec3, uint32> object_book;
	Chunk terrain[30][30];
	int height_map[480][480];

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
};

