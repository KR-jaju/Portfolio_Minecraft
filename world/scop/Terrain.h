#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"

class Chunk;

class Terrain
{
public:
	Terrain(HWND hwnd, uint32 width, uint32 height);
	~Terrain();
	void setRender();
	void Render(Mat const& proj, Mat const& view, vec3 const& pos);
	void createHeightMap();
	void terrainsetVerticesAndIndices();
	void setSightChunk(int cnt);
	void terrainUpdate(float x, float z);

public: // test func
	void readTerrainForTest();

private:
	int checkTerrainBoundary(float x, float z) const;
	WorldIndex coordinateToIndex(
		float x, 
		float y,
		float z
	) const;
	int relocateTerrain(float x, float z, int flag);
	void allocateTerrain(int flag);
	void allocateTerrainLeft(int flag);
	void allocateTerrainRiht(int flag);
	void allocateTerrainBack(int flag);
	void allocateTerrainFront(int flag);
private:
	set<string> file_book;
	map<vec3, uint32> object_book;
	map<vec3, shared_ptr<Chunk>> tmp_storage;
	shared_ptr<Chunk> terrain[30][30];
	PerlinNoise perlin_noise;
	vec2 start_pos;
	vec2 end_pos;
	int size_w = 8;
	int size_h = 8;
	int sight_r = 1;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
	shared_ptr<SamplerState> sampler_state;
};

