#pragma once

#include "Chunk.h"
#include "TerrainDB.h"
#include "EntityReigstry.h"
#include "TerrainGenerator.h"
#include "SubchunkMesh.h"
#include <unordered_set>
#include <optional>

struct ChunkRegistry
{
	ChunkRegistry();

	ivec2 addressing_offset;
	int addressing_half_stride;
	int addressing_stride;
	std::vector<std::shared_ptr<Chunk const>> chunks; // toroidal addressing (for simulation)
	std::vector<SubchunkMesh> subchunk_meshes;
	std::unordered_map<ivec2, std::shared_ptr<Chunk>> changed_chunks; // changed chunks (setBlock)
	std::unordered_set<ivec3> dirty_subchunks; // which subchunk needs mesh update?
	
	std::shared_ptr<Chunk const> getChunk(ivec2 chunk_idx) const;
	std::shared_ptr<Chunk const> getChunk(int chunk_x, int chunk_z) const;
	void	setChunk(ivec2 chunk_idx, std::shared_ptr<Chunk const> const& chunk);
	void	setChunk(int chunk_x, int chunk_z, std::shared_ptr<Chunk const> const& chunk);

	BlockData getBlock(int x, int y, int z) const;
	void	setBlock(int x, int y, int z, BlockData data);

	SubchunkMesh const& getSubchunkMesh(ivec3 subchunk_idx) const;
	SubchunkMesh const& getSubchunkMesh(int subchunk_x, int subchunk_y, int subchunk_z) const;
	void	setSubchunkMesh(ivec3 subchunk_idx, SubchunkMesh&& mesh);
	void	setSubchunkMesh(int subchunk_x, int subchunk_y, int subchunk_z, SubchunkMesh&& mesh);
};