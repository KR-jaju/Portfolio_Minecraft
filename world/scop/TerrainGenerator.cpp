#include "pch.h"
#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
	: perlin_noise()
{}

void	TerrainGenerator::generate(int chunk_x, int chunk_z, Chunk& dest) const
{
	float const offset = 0.000001f;

	for (int z = 0; z < 16; ++z)
	{
		float u = (chunk_z * 16 - z + offset) / 32.0f;
		for (int x = 0; x < 16; ++x)
		{
			float v = (chunk_x * 16 + x + offset) / 32.0f;
			double sample = this->perlin_noise.getNoise2D(u, v, 3, 0.5);
			int16 height = static_cast<int16>((sample + 0.8) * 0.5 * 50.0f);

			for (int y = 0; y < height; ++y)
			{
				float w = y / 16.0f;
				sample = this->perlin_noise.getNoise3D(u, w, v, 3, 0.5);
				if (sample < 0.1 || y < 1)
					dest.setBlock(x, y, z, 1);
				else
					dest.setBlock(x, y, z, 0);
			}
		}
	}
}