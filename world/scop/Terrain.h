#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"

class Chunk;

class Terrain
{
public:
	Terrain(int size_w, int size_h);
	~Terrain();
	void setRender();
	void Render(Mat const& proj, Mat const& view, vec3 const& pos);
	void createHeightMap();
	void terrainsetVerticesAndIndices();
	void setSightChunk(int cnt);

public: // test func & temp func
	void readTerrainForTest();
	void setGraphics(HWND hwnd, uint32 width, uint32 height);

private:
	int checkTerrainBoundary(float x, float z) const;
	pair<int, int> getIndexXZ(float w_x, float w_z);

private:
	set<string> file_book;
	map<vec3, uint32> object_book;
	map<vec3, shared_ptr<Chunk>> tmp_storage;
	shared_ptr<Chunk> chunks[30][30];
	PerlinNoise perlin_noise;
	int size_w = 8;
	int size_h = 8;
	int sight_r = 1;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
	shared_ptr<SamplerState> sampler_state;

private:
	int* terrain;
	int* height_map;
	vec2 start_pos;
	vec2 end_pos;
};

