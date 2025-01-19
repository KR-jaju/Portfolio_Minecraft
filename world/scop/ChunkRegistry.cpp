#include "pch.h"
#include "ChunkRegistry.h"

#include "Entity.h"
#include "Player.h"
#include "ChunkMeshBuilder.h"

ChunkRegistry::ChunkRegistry(TerrainDB& db, TextureRegistry& texture_registry, EntityRegistry& entity_registry)
	: terrain_db(db),
	texture_registry(texture_registry),
	entity_registry(entity_registry),
	render_distance(12),
	simulation_distance(8),
	center(0, 0)
{
	this->initializeChunks();
}

bool	ChunkRegistry::isLoadedChunk(int chunk_x, int chunk_z)
{
	int load_distance = this->render_distance + 1;
	int x = chunk_x - this->center.x;
	int z = chunk_z - this->center.y;

	return (-load_distance <= x && x <= load_distance) && (-load_distance <= z && z <= load_distance);
}

bool	ChunkRegistry::isSimulationChunk(int chunk_x, int chunk_z)
{
	int x = chunk_x - this->center.x;
	int z = chunk_z - this->center.y;
	
	return (-this->simulation_distance <= x && x <= this->simulation_distance) && (-this->simulation_distance <= z && z <= this->simulation_distance);
}

bool	ChunkRegistry::isRenderingChunk(int chunk_x, int chunk_z)
{
	int x = chunk_x - this->center.x;
	int z = chunk_z - this->center.y;

	return (-this->render_distance <= x && x <= this->render_distance) && (-this->simulation_distance <= z && z <= this->simulation_distance);
}

void	ChunkRegistry::update()
{
	Entity& player = this->entity_registry.getPlayer();

	this->updateChunks(player.getChunkIndex());
}

void	ChunkRegistry::setBlock(int x, int y, int z, BlockData data)
{
	int chunk_x = (x + ((x < 0) ? -15 : 0)) / 16;
	int chunk_z = (z + ((z < 0) ? -15 : 0)) / 16;
	Chunk& chunk = this->getChunk(chunk_x, chunk_z);

	chunk.setBlock(x - chunk_x * 16, y, z - chunk_z * 16, data);
	for (Callback* callback : this->callback_list)
		callback->onBlockChanged({ x, y, z });
}

BlockData	ChunkRegistry::getBlock(int x, int y, int z) const
{
	int chunk_x = (x + ((x < 0) ? -15 : 0)) / 16;
	int chunk_z = (z + ((z < 0) ? -15 : 0)) / 16;
	Chunk const& chunk = this->getChunk(chunk_x, chunk_z);

	return (chunk.getBlock(x - chunk_x * 16, y, z - chunk_z * 16));
}

int	ChunkRegistry::getRenderDistance() const
{
	return (this->render_distance);
}

int ChunkRegistry::getSimulationDistance() const
{
	return (this->simulation_distance);
}

Chunk const& ChunkRegistry::getChunk(int chunk_x, int chunk_z) const
{
	int load_distance = this->render_distance + 1;
	int offset_x = chunk_x - this->center.x;
	int offset_z = chunk_z - this->center.y;

	if (offset_x < -load_distance || load_distance < offset_x)
		throw std::runtime_error("Index out of bound!");
	if (offset_z < -load_distance || load_distance < offset_z)
		throw std::runtime_error("Index out of bound!");
	int load_stride = load_distance * 2 + 1;
	int u = (chunk_x % load_stride + load_stride) % load_stride;
	int v = (chunk_z % load_stride + load_stride) % load_stride;

	return this->loaded[u + v * load_stride];
}

Chunk& ChunkRegistry::getChunk(int chunk_x, int chunk_z)
{
	int load_distance = this->render_distance + 1;
	int offset_x = chunk_x - this->center.x;
	int offset_z = chunk_z - this->center.y;

	if (offset_x < -load_distance || load_distance < offset_x)
		throw std::runtime_error("Index out of bound!");
	if (offset_z < -load_distance || load_distance < offset_z)
		throw std::runtime_error("Index out of bound!");
	int load_stride = load_distance * 2 + 1;
	int u = (chunk_x % load_stride + load_stride) % load_stride;
	int v = (chunk_z % load_stride + load_stride) % load_stride;

	return this->loaded[u + v * load_stride];
}

bool	ChunkRegistry::isLoaded(int chunk_x, int chunk_z) const
{
	int load_distance = this->render_distance + 1;
	int x = chunk_x - this->center.x;
	int z = chunk_z - this->center.y;

	return (-load_distance <= x && x <= load_distance) && (-load_distance <= z && z <= load_distance);
}

void	ChunkRegistry::addCallback(Callback* callback)
{
	this->callback_list.emplace_back(callback);
}










void	ChunkRegistry::initializeChunks()
{
	int const load_distance = this->render_distance + 1;
	int const load_stride = (2 * load_distance + 1);

	this->loaded.reserve(load_stride * load_stride);
	for (int z = -load_distance; z <= load_distance; ++z)
	{
		for (int x = -load_distance; x <= load_distance; ++x)
		{
			this->loaded.emplace_back(x, z);
		}
	} // loaded를 일단 채움
	for (int z = -load_distance; z <= load_distance; ++z) // TODO: 여긴 범위가 카메라 기준이어야 함.
	{
		for (int x = -load_distance; x <= load_distance; ++x)
		{
			Chunk& chunk = this->getChunk(x, z);

			if (this->terrain_db.hasChunk(x, z)) // 있으면 로드해야함
				continue;
			this->terrain_generator.generate(x, z, chunk);
			//this->terrain_db.storeChunk(x, z, chunk);
		}
	} // 없는 청크를 생성함
}

void	ChunkRegistry::updateChunks(ivec2 new_offset)
{
	if (this->center.x == new_offset.x && this->center.y == new_offset.y)
		return;
	int load_distance = this->render_distance + 1;

	for (int z = -load_distance; z <= load_distance; ++z)
	{
		for (int x = -load_distance; x <= load_distance; ++x)
		{
			if (!this->isLoadedChunk(new_offset.x + x, new_offset.y + z)) // 새로 로드돼야하는 경우
				this->swapChunk(x, z, new_offset);
		}
	}
	this->center = new_offset;
}

void	ChunkRegistry::loadChunk(int chunk_x, int chunk_z, Chunk& dest)
{
	if (this->terrain_db.loadChunk(chunk_x, chunk_z, dest))
		return;
	this->terrain_generator.generate(chunk_x, chunk_z, dest);
}

// 
void	ChunkRegistry::swapChunk(int offset_x, int offset_z, ivec2 new_offset)
{
	int chunk_x = this->center.x + offset_x;
	int chunk_z = this->center.y + offset_z;
	Chunk& chunk = this->getChunk(chunk_x, chunk_z);

	this->terrain_db.storeChunk(chunk_x, chunk_z, chunk);
	this->loadChunk(new_offset.x + offset_x, new_offset.y + offset_z, chunk);
	for (Callback* callback : this->callback_list)
		callback->onChunkLoaded({ chunk_x, chunk_z });
}
#include <chrono>

//void	ChunkRegistry::updateChunkMesh()
//{
//	while (!this->dirty_chunks.empty())
//	{
//		std::pair<int, int> idx = this->dirty_chunks.front();
//		int cx = idx.first;
//		int cz = idx.second;
//		std::chrono::steady_clock::time_point timestamp = std::chrono::high_resolution_clock::now();
//
//		this->dirty_chunks.pop();
//		if (!this->isRenderingChunk(idx.first, idx.second))
//			continue;
//		this->thread_pool.enqueue([this, cx, cz, timestamp] {
//			std::vector<ChunkVertex> vertices;
//			std::vector<uint32> indices;
//			ChunkMeshBuilder mesh_builder(
//				this->texture_registry,
//				this->getChunk(cx, cz),
//				this->getChunk(cx + 1, cz),
//				this->getChunk(cx - 1, cz),
//				this->getChunk(cx, cz + 1),
//				this->getChunk(cx, cz - 1)
//			);
//			mesh_builder.buildOpaqueMesh(vertices, indices);
//			{
//				RenderChunk& dest = this->getRenderChunk(cx, cz);
//
//				dest.setMesh(std::move(vertices), std::move(indices));
//			}
//			});
//	}
//	this->thread_pool.execute();
//}
