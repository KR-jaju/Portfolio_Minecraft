#pragma once

#include <vector>
#include "EntityVertex.h"
#include "Buffer.h"

class EntityGeometry
{
public:
	EntityGeometry(ComPtr<ID3D11Device> device, std::vector<EntityVertex> const& vertices, std::vector<uint32> indices);
	Buffer<EntityVertex> const& getVertexBuffer() const;
	Buffer<uint32> const& getIndexBuffer() const;
private:
	Buffer<EntityVertex> vertex_buffer;
	Buffer<uint32> index_buffer;
};