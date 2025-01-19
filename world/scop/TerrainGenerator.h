#pragma once

#include "Chunk.h"
#include "PerlinNoise.h"

class TerrainGenerator
{
public:
	void	generate(int chunk_x, int chunk_z, Chunk& dest);
private:
	PerlinNoise perlin_noise;
};
