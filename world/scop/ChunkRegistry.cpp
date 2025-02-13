#include "pch.h"
#include "ChunkRegistry.h"

ChunkRegistry::ChunkRegistry()
	: addressing_offset(0, 0),
	addressing_half_stride(9),
	addressing_stride(this->addressing_half_stride * 2 + 1),
	chunks(this->addressing_stride * this->addressing_stride, nullptr),
	subchunk_meshes(),
	changed_chunks(), dirty_subchunks()
{
	for (int i = 0; i < this->addressing_stride * this->addressing_stride * 16; ++i)
	{
		this->subchunk_meshes.emplace_back();
	}
	
}


std::shared_ptr<Chunk const>	ChunkRegistry::getChunk(ivec2 chunk_idx) const
{
	ivec2 const offset = this->addressing_offset;
	int const half_stride = this->addressing_half_stride;

	if (std::abs(chunk_idx.x - offset.x) > half_stride || std::abs(chunk_idx.y - offset.y) > half_stride)
		return nullptr;
	{
		auto it = this->changed_chunks.find(chunk_idx);

		if (it != this->changed_chunks.end())
			return (it->second);
	}
	int const load_stride = this->addressing_stride;
	int const u = (chunk_idx.x % load_stride + load_stride) % load_stride;
	int const v = (chunk_idx.y % load_stride + load_stride) % load_stride;

	return this->chunks[u + v * load_stride];
}

std::shared_ptr<Chunk const> ChunkRegistry::getChunk(int chunk_x, int chunk_z) const
{
	return (this->getChunk(ivec2(chunk_x, chunk_z)));
} // TODO: 제대로 구현하기

void	ChunkRegistry::setChunk(ivec2 chunk_idx, std::shared_ptr<Chunk const> const& chunk)
{
	ivec2 const offset = this->addressing_offset;
	int const half_stride = this->addressing_half_stride;

	if (std::abs(chunk_idx.x - offset.x) > half_stride || std::abs(chunk_idx.y - offset.y) > half_stride)
		throw std::runtime_error("Invalid subchunk index");
	int const load_stride = this->addressing_stride;
	int const u = (chunk_idx.x % load_stride + load_stride) % load_stride;
	int const v = (chunk_idx.y % load_stride + load_stride) % load_stride;

	this->chunks[u + v * load_stride] = chunk;
}

void	ChunkRegistry::setChunk(int chunk_x, int chunk_z, std::shared_ptr<Chunk const> const& chunk)
{
	ivec2 const offset = this->addressing_offset;
	int const half_stride = this->addressing_half_stride;

	if (std::abs(chunk_x - offset.x) > half_stride || std::abs(chunk_z - offset.y) > half_stride)
		throw std::runtime_error("Invalid subchunk index");
	int const load_stride = this->addressing_stride;
	int const u = (chunk_x % load_stride + load_stride) % load_stride;
	int const v = (chunk_z % load_stride + load_stride) % load_stride;

	this->chunks[u + v * load_stride] = chunk;
}

BlockData ChunkRegistry::getBlock(int x, int y, int z) const
{
	ivec2 chunk_idx = ivec2((x >= 0) ? (x / 16) : ((x - 15) / 16), (z >= 0) ? (z / 16) : ((z - 15) / 16));
	std::shared_ptr<Chunk const> const& chunk = this->getChunk(chunk_idx);

	if (chunk == nullptr)
		return BlockData();
	return chunk->getBlock(x - chunk_idx.x * 16, y, z - chunk_idx.y * 16);
}

void	ChunkRegistry::setBlock(int x, int y, int z, BlockData data)
{
	ivec2 const chunk_idx = ivec2((x >= 0) ? (x / 16) : ((x - 15) / 16), (z >= 0) ? (z / 16) : ((z - 15) / 16));
	ivec2 const offset = this->addressing_offset;
	int const half_stride = 12; // 반드시 100% 로드된 청크들만 setBlock가능

	if (std::abs(chunk_idx.x - offset.x) > half_stride || std::abs(chunk_idx.y - offset.y) > half_stride)
		return;
	auto it = this->changed_chunks.find(chunk_idx);

	if (it == this->changed_chunks.end())
		it = this->changed_chunks.emplace(chunk_idx, std::make_shared<Chunk>(*this->getChunk(chunk_idx))).first;
	it->second->setBlock(x - chunk_idx.x * 16, y, z - chunk_idx.y * 16, data);
} //TODO : dirty_subchunks 플래그도 업데이트 해야함.

SubchunkMesh const& ChunkRegistry::getSubchunkMesh(ivec3 subchunk_idx) const
{
	ivec2 const offset = this->addressing_offset;
	int const half_stride = this->addressing_half_stride;

	if (std::abs(subchunk_idx.x - offset.x) > half_stride || std::abs(subchunk_idx.z - offset.y) > half_stride)
		return {};
	int const load_stride = this->addressing_stride;
	int const u = (subchunk_idx.x % load_stride + load_stride) % load_stride;
	int const v = (subchunk_idx.z % load_stride + load_stride) % load_stride;

	return this->subchunk_meshes[(u + v * load_stride) * 16 + subchunk_idx.y];
}

SubchunkMesh const& ChunkRegistry::getSubchunkMesh(int subchunk_x, int subchunk_y, int subchunk_z) const
{
	return this->getSubchunkMesh(ivec3(subchunk_x, subchunk_y, subchunk_z));
}

void	ChunkRegistry::setSubchunkMesh(ivec3 subchunk_idx, SubchunkMesh&& mesh)
{
	ivec2 const offset = this->addressing_offset;
	int const half_stride = this->addressing_half_stride;

	if (std::abs(subchunk_idx.x - offset.x) > half_stride || std::abs(subchunk_idx.z - offset.y) > half_stride)
		throw std::runtime_error("Invalid subchunk index");
	int const load_stride = this->addressing_stride;
	int const u = (subchunk_idx.x % load_stride + load_stride) % load_stride;
	int const v = (subchunk_idx.z % load_stride + load_stride) % load_stride;

	this->subchunk_meshes[(u + v * load_stride) * 16 + subchunk_idx.y] = std::move(mesh);
}

void	ChunkRegistry::setSubchunkMesh(int subchunk_x, int subchunk_y, int subchunk_z, SubchunkMesh&& mesh)
{
	this->setSubchunkMesh(ivec3(subchunk_x, subchunk_y, subchunk_z), std::move(mesh));
}





//
//#include "Entity.h"
//#include "Player.h"
//#include "ChunkMeshBuilder.h"
//
//ChunkRegistry::ChunkRegistry()
//	: load_size(12), center(0, 0)
//{
//	this->initializeChunks();
//}
//
////void	ChunkRegistry::update()
////{
////	Entity& player = this->entity_registry.getPlayer();
////
////	this->updateChunks(player.getChunkIndex());
////}
//
//void	ChunkRegistry::setBlock(int x, int y, int z, BlockData data)
//{
//	int chunk_x = (x + ((x < 0) ? -15 : 0)) / 16;
//	int chunk_z = (z + ((z < 0) ? -15 : 0)) / 16;
//	Chunk& chunk = this->getChunk(chunk_x, chunk_z);
//
//	chunk.setBlock(x - chunk_x * 16, y, z - chunk_z * 16, data);
//	for (Callback* callback : this->callback_list)
//		callback->onBlockChanged({ x, y, z });
//}
//
//BlockData	ChunkRegistry::getBlock(int x, int y, int z) const
//{
//	int chunk_x = (x + ((x < 0) ? -15 : 0)) / 16;
//	int chunk_z = (z + ((z < 0) ? -15 : 0)) / 16;
//	Chunk const& chunk = this->getChunk(chunk_x, chunk_z);
//
//	return (chunk.getBlock(x - chunk_x * 16, y, z - chunk_z * 16));
//}
//
//int ChunkRegistry::getLoadSize() const
//{
//	return (this->load_size);
//}
//
//Chunk const& ChunkRegistry::getChunk(int chunk_x, int chunk_z) const
//{
//	int offset_x = chunk_x - this->center.x;
//	int offset_z = chunk_z - this->center.y;
//
//	if (offset_x < -this->load_size || this->load_size < offset_x)
//		throw std::runtime_error("Index out of bound!");
//	if (offset_z < -this->load_size || this->load_size < offset_z)
//		throw std::runtime_error("Index out of bound!");
//	int load_stride = this->load_size * 2 + 1;
//	int u = (chunk_x % load_stride + load_stride) % load_stride;
//	int v = (chunk_z % load_stride + load_stride) % load_stride;
//
//	return this->loaded[u + v * load_stride];
//}
//
//Chunk& ChunkRegistry::getChunk(int chunk_x, int chunk_z)
//{
//	int offset_x = chunk_x - this->center.x;
//	int offset_z = chunk_z - this->center.y;
//
//	if (offset_x < -this->load_size || this->load_size < offset_x)
//		throw std::runtime_error("Index out of bound!");
//	if (offset_z < -this->load_size || this->load_size < offset_z)
//		throw std::runtime_error("Index out of bound!");
//	int load_stride = this->load_size * 2 + 1;
//	int u = (chunk_x % load_stride + load_stride) % load_stride;
//	int v = (chunk_z % load_stride + load_stride) % load_stride;
//
//	return this->loaded[u + v * load_stride];
//}
//
//bool	ChunkRegistry::isLoaded(int chunk_x, int chunk_z) const
//{
//	int x = chunk_x - this->center.x;
//	int z = chunk_z - this->center.y;
//
//	return (-this->load_size <= x && x <= this->load_size) && (-this->load_size <= z && z <= this->load_size);
//}
//
////void	ChunkRegistry::initializeChunks()
////{
////	int const load_stride = (2 * this->load_size + 1);
////
////	this->loaded.reserve(load_stride * load_stride);
////	for (int z = -this->load_size; z <= this->load_size; ++z)
////	{
////		for (int x = -this->load_size; x <= this->load_size; ++x)
////		{
////			this->loaded.emplace_back(x, z);
////		}
////	} // loaded를 일단 채움
////	for (int z = -this->load_size; z <= this->load_size; ++z) // TODO: 여긴 범위가 카메라 기준이어야 함.
////	{
////		for (int x = -this->load_size; x <= this->load_size; ++x)
////		{
////			Chunk& chunk = this->getChunk(x, z);
////
////			if (this->terrain_db.hasChunk(x, z)) // 있으면 로드해야함
////				continue;
////			this->terrain_generator.generate(x, z, chunk);
////			//this->terrain_db.storeChunk(x, z, chunk);
////		}
////	} // 없는 청크를 생성함
////}
//
////void	ChunkRegistry::updateChunks(ivec2 new_offset)
////{
////	if (this->center.x == new_offset.x && this->center.y == new_offset.y)
////		return;
////	for (int z = -this->load_size; z <= this->load_size; ++z)
////	{
////		for (int x = -this->load_size; x <= this->load_size; ++x)
////		{
////			if (!this->isLoaded(new_offset.x + x, new_offset.y + z)) // 새로 로드돼야하는 경우
////				this->swapChunk(x, z, new_offset);
////		}
////	}
////	this->center = new_offset;
////}
////
////void	ChunkRegistry::loadChunk(int chunk_x, int chunk_z, Chunk& dest)
////{
////	if (this->terrain_db.loadChunk(chunk_x, chunk_z, dest))
////		return;
////	this->terrain_generator.generate(chunk_x, chunk_z, dest);
////}
////
////void	ChunkRegistry::swapChunk(int offset_x, int offset_z, ivec2 new_offset)
////{
////	int chunk_x = this->center.x + offset_x;
////	int chunk_z = this->center.y + offset_z;
////	Chunk& chunk = this->getChunk(chunk_x, chunk_z);
////
////	this->terrain_db.storeChunk(chunk_x, chunk_z, chunk);
////	this->loadChunk(new_offset.x + offset_x, new_offset.y + offset_z, chunk);
////	for (Callback* callback : this->callback_list)
////		callback->onChunkLoaded({ chunk_x, chunk_z });
////}
