#pragma once

#include "WorldUtils.h"

class Chunk;

class MapUtils
{
public:
	MapUtils(int size_w, int size_h, HWND hwnd, 
		UINT w_width, UINT w_heigh);
	~MapUtils();
public:
	vec3 intersectionRayAndPlane(
		vec3 const& r_pos,
		vec3 const& r_dir,
		vec3 const& p_pos,
		vec3 const& p_dir
	);
	bool inChunkBoundary(int x, int y, int z);
	bool inChunkBoundary(Index3 const& bidx);

public:
	int findBlock(Index2 const& c_idx, int x, int y, int z) const;
	int findBlock(Index2 const& c_idx, Index3 const& b_idx) const;
	void addBlock(Index2 const& c_idx, int x, int y, int z, int type);
	void addBlock(Index2 const& c_idx, Index3 const& b_idx, int type);
	int findHeight(Index2 const& c_idx, Index2 const& h_idx) const;
	int findHeight(Index2 const& c_idx, Index3 const& b_idx) const;
	int findHeight(Index2 const& c_idx, int x, int z) const;
	void setHeight(Index2 const& c_idx, Index2 const& b_idx, int h);
	void setHeight(Index2 const& c_idx, int x, int z, int h);
	uint8 findLight(Index2 const& c_idx, int x, int y, int z) const;
	uint8 findLight(Index2 const& c_idx, Index3 const& b_idx) const;
	void setLight(Index2 const& c_idx, int x, int y, int z, uint8 type);
	void setLight(Index2 const& c_idx, Index3 const& b_idx, uint8 type);
	void setWater(Index2 const& c_idx, Index3 const& b_idx, bool water);
	void setWater(Index2 const& c_idx, int x, int y, int z, bool water);
	bool getWater(Index2 const& c_idx, Index3 const& b_idx);
	bool getWater(Index2 const& c_idx, int x, int y, int z);

public:
	WorldIndex getBlockIndex(float x, float y, float z) const;
	WorldIndex pickBlock(vec3 r_pos, vec3 r_dir);
	Index2 findChunkIndex(int w_x, int w_z) const;
	Index2 getChunkIndex(int w_x, int w_z) const;

public:
	int size_w; // 맵 세로 크기
	int size_h; // 맵 가로 크기
	Index2 s_pos; // 맵의 시작 위치
	Index2 sv_pos; // 실제로 보이는 부분의 시작위치
	Index2 ev_pos; // 실제로 보이는 부분의 끝 위치
	shared_ptr<Chunk> chunks[30][30];
	HWND hWnd;
	UINT width; // 창 가로 크기
	UINT height; // 창 세로 크기
	vec3 directional_light_pos;
	vec3 light_dir;
	vec3 cam_pos; // test

private:
	int* blocks;
	bool* water;
	int* h_map;
	uint8* light_map;
};

