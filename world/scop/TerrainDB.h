#pragma once

#include <mutex>
#include <set>
#include "Chunk.h"
#include "ThreadPool.h"
#include "OrderedMap.h"
#include "TerrainGenerator.h"
#include "StripedMutex.h"
#include "Future.h"

/*
청크의 저장과 로드를 비동기(병렬)로 해주는 클래스.
*/
class TerrainDB
{
public:
	TerrainDB(ThreadPool& thread_pool, std::string const& path);
	// 비동기 로드, future를 사용하면 청크가 로드될 때까지 기다리게 할 수 있음.
	Future<std::shared_ptr<Chunk const>>	loadChunkAsync(ivec2 chunk_idx);
	// 비동기 스토어
	void  storeChunk(ivec2 chunk_idx, std::shared_ptr<Chunk const> const& chunk);
private:
	ThreadPool& thread_pool;
	TerrainGenerator terrain_generator;
	std::string path_base;
	StripedMutex<ivec2, 8> file_mutex;

	std::shared_ptr<Chunk const>  loaderMain(ivec2 chunk_idx);
	void  writerMain(ivec2 chunk_idx, std::shared_ptr<Chunk const> const& chunk);
	std::string getPath(ivec2 chunk_idx);
};