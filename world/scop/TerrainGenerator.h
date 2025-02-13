#pragma once

#include "Chunk.h"
#include "PerlinNoise.h"

class TerrainGenerator
{
public:
	TerrainGenerator();
	void	generate(int chunk_x, int chunk_z, Chunk& dest) const;
private:
	PerlinNoise perlin_noise;
};
