#pragma once

#include "Chunk.h"

class TerrainDB
{
public:
	void	storeChunk(int chunk_x, int chunk_z, Chunk const& chunk);
	bool	loadChunk(int chunk_x, int chunk_z, Chunk& dest);
	bool	hasChunk(int chunk_x, int chunk_z) const;
private:

};
