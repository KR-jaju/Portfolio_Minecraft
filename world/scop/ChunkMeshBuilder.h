#pragma once

#include "Chunk.h"
#include "ConstantRegistry.h"

class ChunkMeshBuilder
{
public:
	ChunkMeshBuilder(ConstantRegistry const& constant_registry, Chunk const& chunk, Chunk const& east, Chunk const& west, Chunk const& north, Chunk const& south);

	void	buildOpaqueMesh(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices);
private:
	//ConstantRegistry const& constant_reg
	std::vector<BlockTextureData> const& block_texture_data;
	Chunk const& chunk;
	Chunk const& east;
	Chunk const& west;
	Chunk const& north;
	Chunk const& south;

	BlockData	getBlock(int x, int y, int z) const;

	void	buildOpaqueBlock(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices, int x, int y, int z, BlockData block_data);
	//void	buildTransparentBlock

	void	buildCube(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices, float x, float y, float z, BlockTextureData const& textures);
};

/*
복사를 한다면 락을 계속 걸 필요가 없다.
*/