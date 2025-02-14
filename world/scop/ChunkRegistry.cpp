#include "pch.h"
#include "ChunkRegistry.h"

ChunkRegistry::ChunkRegistry()
	: addressing_offset(0, 0),
	addressing_half_stride(12),
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
	int const load_stride = this->addressing_stride;
	int const u = (chunk_idx.x % load_stride + load_stride) % load_stride;
	int const v = (chunk_idx.y % load_stride + load_stride) % load_stride;

	return this->chunks[u + v * load_stride];
}

std::shared_ptr<Chunk const> ChunkRegistry::getChunk(int chunk_x, int chunk_z) const
{
	return (this->getChunk(ivec2(chunk_x, chunk_z)));
}

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
	int const half_stride = 6; // 반드시 100% 로드된 청크들만 setBlock가능

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
