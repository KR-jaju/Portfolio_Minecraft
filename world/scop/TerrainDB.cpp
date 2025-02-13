 #include "pch.h"
#include "TerrainDB.h"
#include <fstream>
#include "Promise.h"

TerrainDB::TerrainDB(ThreadPool& thread_pool, std::string const& path)
	: thread_pool(thread_pool),
	terrain_generator(),
	path_base(path) {}

Future<std::shared_ptr<Chunk const>>	TerrainDB::loadChunkAsync(ivec2 chunk_idx)
{
	Promise<std::shared_ptr<Chunk const>> promise;
	Future<std::shared_ptr<Chunk const>> future = promise.getFuture();

	this->thread_pool.enqueue(ThreadPool::Priority::High, std::move([this, promise = std::move(promise), chunk_idx]() mutable // promise가 non-const로 다뤄져야함
	{
		auto chunk = this->loaderMain(chunk_idx);

		promise.setValue(std::move(chunk));
	}));
	return future;
}

// 비동기 스토어
void  TerrainDB::storeChunk(ivec2 chunk_idx, std::shared_ptr<Chunk const> const& chunk)
{
	this->thread_pool.enqueue(ThreadPool::Priority::High, [this, chunk_idx, chunk]()
	{
		this->writerMain(chunk_idx,chunk);
	});
}

std::shared_ptr<Chunk const>	TerrainDB::loaderMain(ivec2 chunk_idx)
{
	static constexpr int CHUNK_SIZE = 16 * 256 * 16;
	std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();

	std::unique_lock<std::mutex> file_lock(this->file_mutex[chunk_idx]);
	std::string const& filename = this->getPath(chunk_idx);
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file)
	{
		size_t fileSize = file.tellg();
		if (fileSize != CHUNK_SIZE)
			throw std::runtime_error("파일 크기가 올바르지 않습니다.");
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(chunk->block_data), CHUNK_SIZE);
	}
	else
	{ // 파일이 없으면 TerrainGenerator를 사용하여 새로 생성
		this->terrain_generator.generate(chunk_idx.x, chunk_idx.y, *chunk);
	}
	return (chunk);
}

void TerrainDB::writerMain(ivec2 chunk_idx, std::shared_ptr<Chunk const> const& chunk)
{
	static constexpr int CHUNK_SIZE = 16 * 256 * 16;

	std::unique_lock<std::mutex> file_lock(this->file_mutex[chunk_idx]);
	std::string const& filename = this->getPath(chunk_idx);
	std::ofstream file(filename, std::ios::binary);

	if (!file)
		throw std::runtime_error("파일 생성 실패");
	file.write(reinterpret_cast<char const*>(chunk->block_data), CHUNK_SIZE);
}

std::string TerrainDB::getPath(ivec2 chunk_idx)
{
	return this->path_base + "chunk_" + std::to_string(chunk_idx.x) + "_" + std::to_string(chunk_idx.y) + ".dat";
}
