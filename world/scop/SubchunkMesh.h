#pragma once

#include <vector>

class Graphics;

class SubchunkMesh
{
public:
	SubchunkMesh();
	SubchunkMesh(std::vector<ChunkVertex>&& vertices, std::vector<uint32>&& indices);

	void	draw(Graphics& graphics) const;
private:
	std::vector<ChunkVertex> vertices;
	std::vector<uint32> indices;
	mutable ComPtr<ID3D11Buffer> vertex_buffer;
	mutable ComPtr<ID3D11Buffer> index_buffer;
};