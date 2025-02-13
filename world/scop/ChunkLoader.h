#pragma once

#include "ThreadPool.h"

class ChunkLoader
{
public:
	ChunkLoader(ThreadPool& thread_pool);
private:
	void	loadAsync();
};