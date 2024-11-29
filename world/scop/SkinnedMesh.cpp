#include "pch.h"
#include "SkinnedMesh.h"


SkinnedMesh::SkinnedMesh(ComPtr<ID3D11Device> device, Model const& model)
	: vertices(model.vertices),
	indices(model.indices),
	bindposes(model.bindposes),
	topological_order(model.topological_order),
	vertex_buffer(device, model.vertices.data(), model.vertices.size(), D3D11_BIND_VERTEX_BUFFER),
	index_buffer(device, model.indices.data(), model.indices.size(), D3D11_BIND_INDEX_BUFFER)
{

}



Buffer<EntityVertex> const& SkinnedMesh::getVertexBuffer() const
{
	return (this->vertex_buffer);
}

Buffer<uint32> const& SkinnedMesh::getIndexBuffer() const
{
	return (this->index_buffer);
}

std::vector<int> const& SkinnedMesh::getBoneOrder() const
{
	return (this->topological_order);
}
