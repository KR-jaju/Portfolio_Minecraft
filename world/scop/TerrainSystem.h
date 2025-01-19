#pragma once

#include "WorldUtils.h"
#include "PerlinNoise.h"

#include "ChunkRegistry.h"
#include "ChunkMeshRegistry.h"
#include "Entity.h"
#include "TerrainDB.h"

class MapUtils;

class TerrainSystem
{
public:
	TerrainSystem(MapUtils* minfo);
	TerrainSystem(EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, ChunkMeshRegistry& chunk_mesh_registry);

	void fillChunk(Index2 const& c_idx, Index2 const& c_pos);
	void createHeighMap();

	void update();

private:
	EntityRegistry& entity_registry;
	ChunkRegistry& chunk_registry;
	ChunkMeshRegistry& chunk_mesh_registry;

	//MapUtils* m_info;
	//PerlinNoise perlin_noise;
};

