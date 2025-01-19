#include "pch.h"
#include "ChunkMeshRegistry.h"
#include "ChunkMeshBuilder.h"
#include "SubchunkSnapshot.h"

ChunkMeshRegistry::ChunkMeshRegistry(TextureRegistry& texture_registry, ChunkRegistry& chunk_registry)
	: texture_registry(texture_registry),
	chunk_registry(chunk_registry),
	thread_pool(4)
{
	int load_distance = chunk_registry.getRenderDistance();
	
	for (int z = -load_distance; z <= load_distance; ++z)
	{
		for (int x = -load_distance; x <= load_distance; ++x)
		{
			for (int y = 0; y < 16; ++y)
				this->change_set.insert({ x, y, z });
		}
	}
}

void	ChunkMeshRegistry::onBlockChanged(ivec3 position)
{
	int chunk_x = (position.x < 0) ? (position.x / 16 - 1) : (position.x / 16);
	int chunk_y = (position.y < 0) ? (position.y / 16 - 1) : (position.y / 16);
	int chunk_z = (position.z < 0) ? (position.z / 16 - 1) : (position.z / 16);

	this->change_set.insert({ chunk_x, chunk_y, chunk_z }); // 0~-15 -> -1이어야하는데 버그임
	//TODO : 경계에서 블럭이 변경됐을 경우는 옆 청크도 추가해야함.
}

void	ChunkMeshRegistry::onChunkLoaded(ivec2 chunk_idx)
{
	for (int y = 0; y < 16; ++y)
		this->change_set.insert({ chunk_idx.x, y, chunk_idx.y });
}

void	ChunkMeshRegistry::onChunkUnload(ivec2 chunk_idx)
{
	
}

void	ChunkMeshRegistry::update()
{
	for (ivec3 subchunk_idx : this->change_set)
	{
		if (!this->chunk_registry.isLoaded(subchunk_idx.x, subchunk_idx.z)
			|| !this->chunk_registry.isLoaded(subchunk_idx.x + 1, subchunk_idx.z)
			|| !this->chunk_registry.isLoaded(subchunk_idx.x - 1, subchunk_idx.z)
			|| !this->chunk_registry.isLoaded(subchunk_idx.x, subchunk_idx.z + 1)
			|| !this->chunk_registry.isLoaded(subchunk_idx.x, subchunk_idx.z - 1))
			continue; // 메쉬 생성이 필요한 청크인가?
		SubchunkMeshData& mesh_data = this->subchunk_mesh_cache[subchunk_idx];
		std::unique_ptr<SubchunkSnapshot> subchunk_snapshot = make_unique<SubchunkSnapshot>();
		int const y_offset = subchunk_idx.y * 16;
		Chunk const& chunk = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z);
		Chunk const& east = this->chunk_registry.getChunk(subchunk_idx.x + 1, subchunk_idx.z);
		Chunk const& west = this->chunk_registry.getChunk(subchunk_idx.x - 1, subchunk_idx.z);
		Chunk const& north = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z + 1);
		Chunk const& south = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z - 1);

		for (int y = 0; y < 16; ++y)
		{
			for (int z = 0; z < 16; ++z)
			{
				for (int x = 0; x < 16; ++x)
				{
					subchunk_snapshot->center[x + z * 16 + y * 16 * 16] = chunk.getBlock(x, y + y_offset, z);
				}
			}
		}
		for (int y = 0; y < 16; ++y)
		{
			for (int z = 0; z < 16; ++z)
			{
				subchunk_snapshot->east[z + y * 16] = east.getBlock(0, y + y_offset, z); //x+
				subchunk_snapshot->west[z + y * 16] = west.getBlock(15, y + y_offset, z); // x-
			}
		}
		for (int z = 0; z < 16; ++z)
		{
			for (int x = 0; x < 16; ++x)
			{
				subchunk_snapshot->up[x + z * 16] = chunk.getBlock(x, y_offset + 16, z); // y+
				subchunk_snapshot->down[x + z * 16] = chunk.getBlock(x, y_offset - 1, z); // y-
			}
		}
		for (int y = 0; y < 16; ++y)
		{
			for (int x = 0; x < 16; ++x)
			{
				subchunk_snapshot->north[x + y * 16] = north.getBlock(x, y + y_offset, 0); // z+
				subchunk_snapshot->south[x + y * 16] = south.getBlock(x, y + y_offset, 15); // z-
			}
		}
		this->thread_pool.enqueue([this, mesh = &mesh_data, snapshot = subchunk_snapshot.release()]() {
			std::vector<ChunkVertex> vertices;
			std::vector<uint32> indices;

			ChunkMeshBuilder mesh_builder(
				this->texture_registry, *snapshot
			);

			mesh_builder.buildOpaqueMesh(vertices, indices);

			mesh->updateMesh(std::move(vertices), std::move(indices));
			delete snapshot;
			});
	}
	this->change_set.clear();
}

std::map<ivec3, SubchunkMeshData, ChunkMeshRegistry::KeyComparator>::iterator ChunkMeshRegistry::begin()
{
	return (this->subchunk_mesh_cache.begin());
}
std::map<ivec3, SubchunkMeshData, ChunkMeshRegistry::KeyComparator>::iterator ChunkMeshRegistry::end()
{
	return (this->subchunk_mesh_cache.end());
}