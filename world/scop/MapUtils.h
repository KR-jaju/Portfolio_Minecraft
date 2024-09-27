#pragma once

#include "WorldUtils.h"

class Chunk;

class MapUtils
{
public:
	MapUtils(int size_w, int size_h);
	~MapUtils();
public:
	vec3 intersectionRayAndPlane(
		vec3 const& r_pos,
		vec3 const& r_dir,
		vec3 const& p_pos,
		vec3 const& p_dir
	);
	bool inChunkBoundary(int x, int y, int z);

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

public:
	WorldIndex getBlockIndex(float x, float y, float z) const;
	WorldIndex pickBlock(vec3 r_pos, vec3 r_dir);
	Index2 findChunkIndex(int w_x, int w_z) const;
	Index2 getChunkIndex(int w_x, int w_z) const;

public:
	int size_w;
	int size_h;
	Index2 s_pos;
	Index2 sv_pos;
	Index2 ev_pos;
	shared_ptr<Chunk> chunks[30][30];

private:
	int* blocks;
	int* h_map;
	uint8* light_map;
};

