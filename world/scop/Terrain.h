#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"
#include "Block.h"
#include <array>

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
	void Render(Mat const& proj, Mat const& view, vec3 const& pos);
	void setRender();

private:
	WorldIndex getBlockIndex(float x, float y, float z) const;
	int checkTerrainBoundary(float x, float z) const;
	Index2 getChunkIndex(int w_x, int w_z) const;
	Index2 findChunkIndex(int w_x, int w_z) const;
	void fillChunk(Index2 const& chunk_pos);
	void createHeightMap();
	void terrainsetVerticesAndIndices();
	void vertexAndIndexGenerator(
		Index2 const& c_idx,
		Index2 const& adj_idx,
		Index3 const& move,
		int dir,
		vector<VertexBlockUV>& vertices,
		vector<uint32>& indices
	);
	int findBlock(Index2 const& c_idx, int x, int y, int z) const;
	int findBlock(Index2 const& c_idx, Index3 const& b_idx) const;
	void addBlock(Index2 const& c_idx, int x, int y, int z, int type);
	void addBlock(Index2 const& c_idx, Index3 const& b_idx, int type);
	int findHeight(Index2 const& c_idx, Index2 const& h_idx) const;
	int findHeight(Index2 const& c_idx, int x, int z) const;
	void setHeight(Index2 const& c_idx, int x, int z, int h);
	void setHeight(Index2 const& c_idx, Index2 const& b_idx, int h);
private:
	int* blocks;
	int* h_map;
	vec2 start_pos;
	Index2 s_pos;
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
	size_t b_arr_size;
	size_t h_arr_size;

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

