#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"

class Chunk;

class Terrain
{
public:
	Terrain(HWND hwnd, uint32 width, uint32 height);
	~Terrain();
	void setViewAndProj(Mat const& view, Mat const& proj);
	void setRender();
	void Render();
	void setCam(Mat view, Mat proj);
	void createHeightMap();
	void terrainsetVerticesAndIndices();
	int checkTerrain(float x, float z) const;
	void setSightChunk(int cnt);


public: // test func
	void readTerrainForTest();

private:
	WorldIndex coordinateToIndex(
		float x, 
		float y,
		float z
	) const;

private:
	Mat view;
	Mat proj;
	set<string> file_book;
	map<vec3, uint32> object_book;
	shared_ptr<Chunk> terrain[30][30];
	int16 height_map[480][480];
	PerlinNoise perlin_noise;
	vec2 start_pos;
	vec2 end_pos;
	int size_w = 4;
	int size_h = 4;
	int sight_r;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
	shared_ptr<SamplerState> sampler_state;

};

