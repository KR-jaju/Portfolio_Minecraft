#pragma once

#include "Graphics.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "WorldUtils.h"

struct RenderChunk
{
	std::vector<ChunkVertex> vertices;
	std::vector<uint32> indices;
	int	chunk_x;
	int chunk_z;
	MVP mvp;
	mutable bool	is_synced;
	mutable std::unique_ptr<Buffer<ChunkVertex>> vertex_buffer;
	mutable std::unique_ptr<Buffer<uint32>> index_buffer;
	mutable std::unique_ptr<ConstantBuffer> mvp_constant;

	RenderChunk(int chunk_x, int chunk_z);

	void	setMesh(std::vector<ChunkVertex>&& vertices, std::vector<uint32>&& indices);

	void	draw(Graphics& graphics) const;
};