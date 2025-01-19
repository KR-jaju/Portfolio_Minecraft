#pragma once

#include <mutex>
#include "Buffer.h"
#include "WorldUtils.h"

class SubchunkMeshData
{
public:
	SubchunkMeshData();
	void	updateMesh(std::vector<ChunkVertex>&& vertices, std::vector<uint32>&& indices);
	void	draw(Graphics& graphics);
private:
	std::vector<ChunkVertex> vertices;
	std::vector<uint32> indices;
	std::mutex mutex;
	bool	is_buffer_synced;

	std::unique_ptr<Buffer<ChunkVertex>> vertex_buffer;
	std::unique_ptr<Buffer<uint32>> index_buffer;

};
