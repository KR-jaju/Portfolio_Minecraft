#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"
#include "Block.h"

class Chunk;

class Terrain
{
public:
	Terrain(int size_w, int size_h, HWND hwnd, 
		UINT width, UINT height);
	~Terrain();
	void setSightChunk(int cnt);
	int getBlock(float x, float y, float z) const;

public: // test func & temp func
	void testFillChunk();
	void Render(Mat const& proj, Mat const& view, vec3 const& pos);
	void setRender();

private:
	WorldIndex getBlockIndex(float x, float y, float z) const;
	int checkTerrainBoundary(float x, float z) const;
	Index2 getChunkIndex(float w_x, float w_z) const;
	void fillChunk(vec2 chunk_pos);
	void createHeightMap();
	void terrainsetVerticesAndIndices();
	void vertexAndIndexGenerator(
		vec2 const& pos,
		Index3 const& move,
		int dir,
		vector<VertexBlockUV>& vertices,
		vector<uint32>& indices
	);
	int getBlockByIndex(Index3 const& b_idx, Index2 const& c_idx) const;
private:
	vec2 start_pos;
	vec2 end_pos;
	set<string> file_book;
	map<vec3, uint32> object_book;
	map<vec3, shared_ptr<Chunk>> tmp_storage;
	shared_ptr<Chunk> chunks[30][30];
	PerlinNoise perlin_noise;
	int size_w;
	int size_h;
	int sight_r;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
};

