#include "pch.h"
#include "EntityGeometry.h"

EntityGeometry::EntityGeometry(ComPtr<ID3D11Device> device, std::vector<EntityVertex> const& vertices, std::vector<uint32> indices)
	: vertex_buffer(device, vertices.data(), vertices.size(), D3D11_BIND_VERTEX_BUFFER),
	index_buffer(device, indices.data(), indices.size(), D3D11_BIND_INDEX_BUFFER)
{

}
Buffer<EntityVertex> const& EntityGeometry::getVertexBuffer() const
{
	return (this->vertex_buffer);
}
Buffer<uint32> const& EntityGeometry::getIndexBuffer() const
{
	return (this->index_buffer);
}