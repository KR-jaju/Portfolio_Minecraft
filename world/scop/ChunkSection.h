#pragma once

#include <vector>
#include <shared_mutex>

struct ChunkSection
{
	std::shared_mutex	mutex;
	std::vector<ChunkVertex> vertices;
	std::vector<uint32> indices;
};
