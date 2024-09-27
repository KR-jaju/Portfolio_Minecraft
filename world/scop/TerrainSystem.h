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

private:
	MapUtils* m_info;
	PerlinNoise perlin_noise;
};

