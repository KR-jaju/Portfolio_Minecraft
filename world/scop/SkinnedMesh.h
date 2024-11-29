#pragma once

#include "ModelResource.h"
#include "Buffer.h"

class SkinnedMesh
{
public:
	SkinnedMesh(ComPtr<ID3D11Device> device, Model const& model);

	Buffer<EntityVertex> const& getVertexBuffer() const;
	Buffer<uint32> const& getIndexBuffer() const;
	std::vector<int> const& getBoneOrder() const;
private:
	std::vector<EntityVertex> vertices;
	std::vector<uint32> indices;
	std::vector<Mat> bindposes;
	std::vector<int> topological_order;

	Buffer<EntityVertex> vertex_buffer;
	Buffer<uint32> index_buffer;
};
