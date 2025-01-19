#pragma once

#include "Chunk.h"
#include "TextureRegistry.h"

#include "SubchunkSnapshot.h"

class ChunkMeshBuilder
{
public:
	ChunkMeshBuilder(TextureRegistry& texture_registry, SubchunkSnapshot& snapshot);

	void	buildOpaqueMesh(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices);
private:
	TextureRegistry& texture_registry;
	SubchunkSnapshot& snapshot;

	BlockData	getBlock(int x, int y, int z) const;

	void	buildOpaqueBlock(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices, int x, int y, int z, BlockData block_data);
	//void	buildTransparentBlock

	void	buildCube(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices, float x, float y, float z, BlockTextureData const& textures);
};

/*
복사를 한다면 락을 계속 걸 필요가 없다.
*/