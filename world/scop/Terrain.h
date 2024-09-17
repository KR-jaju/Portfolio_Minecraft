#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"
#include "Block.h"
#include <array>

class Chunk;

// test
vec3 intersectionRayAndPlane(
	vec3 const& r_pos,
	vec3 const& r_dir,
	vec3 const& p_pos,
	vec3 const& p_dir
);

class Terrain
{
public:
	Terrain(int size_w, int size_h, 
		HWND hwnd, UINT width, 
		UINT height, int fov_chunk, 
		int thread_cnt
	);
	~Terrain();
	void setSightChunk(int cnt);
	void userPositionCheck(float x, float z);
	int16 getHeight(float x, float z) const;
	void putBlock(vec3 const& ray_pos, vec3 const& ray_dir, int type);
	void deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir);
	int getBlock(float x, float y, float z);

public: // test func & temp func
	void Render(Mat const& proj, Mat const& view, vec3 const& pos);
	void setRender();
	void showChunk(Index2 const& c_idx);
	void selectBlockTest(vec3 ray_pos, vec3 ray_dir);

private: //test
	vec3 getBlockIdxToWorld(Index2& c_idx, int x, int y, int z);

private:
	void resetChunk(Index2 const& c_idx);
	void chunksSetVerticesAndIndices(
		vector<Index2> const& v_idx, 
		int st, 
		int ed
	);
	int checkTerrainBoundary(float x, float z) const;
	void fillChunk(Index2 const& c_idx, Index2 const& c_pos);
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

private: // find or add info
	Index2 findChunkIndex(int w_x, int w_z) const;
	Index2 getChunkIndex(int w_x, int w_z) const;
	WorldIndex pickBlock(vec3 r_pos, vec3 r_dir);
	WorldIndex getBlockIndex(float x, float y, float z) const;
	int findBlock(Index2 const& c_idx, int x, int y, int z) const;
	int findBlock(Index2 const& c_idx, Index3 const& b_idx) const;
	void addBlock(Index2 const& c_idx, int x, int y, int z, int type);
	void addBlock(Index2 const& c_idx, Index3 const& b_idx, int type);
	int findHeight(Index2 const& c_idx, Index2 const& h_idx) const;
	int findHeight(Index2 const& c_idx, int x, int z) const;
	void setHeight(Index2 const& c_idx, Index2 const& b_idx, int h);
	void setHeight(Index2 const& c_idx, int x, int z, int h);

private:
	int* blocks;
	int* h_map;
	Index2 s_pos;
	Index2 sv_pos;
	Index2 ev_pos;
	set<string> file_book;
	map<vec3, uint32> object_book;
	map<vec3, shared_ptr<Chunk>> tmp_storage;
	shared_ptr<Chunk> chunks[30][30];
	PerlinNoise perlin_noise;
	int thread_cnt;
	int size_w;
	int size_h;
	int c_fov;

private:
	bool test_flag = false;

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

