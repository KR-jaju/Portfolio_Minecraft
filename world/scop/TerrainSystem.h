#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"

class MapUtils;

class TerrainSystem
{
public:
	TerrainSystem(MapUtils* minfo);
	void fillChunk(Index2 const& c_idx, Index2 const& c_pos);
	void createHeightMap();
	void createTrees();
	void createTrees(vector<Index2>& chunks, int dir);

public: // test
	void setTree(Index2 const& c_idx, int x, int y, int z);

private:
	int getAdjInfo(Index2 const& c_idx, int x, int y, int z, 
		int type, int dir);
	void fillWater(Index2 const& c_idx);
	void makeCave(Index2 const& c_idx, Index2 const& c_pos);
	void makeTree(Index2 const& c_idx);
	void resetChunkHeight(
		Index2 const& c_idx,
		int x, int z, int h
	);
private:
	void checkChunk(
		Index2 const& c_idx,
		int x, int y, int z,
		Index2& res_cidx,
		Index3& res_bidx
	);
	void putTree(Index2 const& c_idx, int x, int y, int z, int type);

private:
	MapUtils* m_info;
	PerlinNoise perlin_noise;
};

