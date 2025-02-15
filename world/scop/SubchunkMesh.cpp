#include "pch.h"
#include "SubchunkMesh.h"
#include "Graphics.h"

SubchunkMesh::SubchunkMesh() : vertices(), indices(), vertex_buffer(nullptr), index_buffer(nullptr) {}

SubchunkMesh::SubchunkMesh(ComPtr<ID3D11Device> device, std::vector<ChunkVertex>&& vertices, std::vector<uint32>&& indices)
	: vertices(std::move(vertices)), indices(std::move(indices)),
	vertex_buffer(nullptr), index_buffer(nullptr)
{
	if (this->vertices.size() == 0 || this->indices.size() == 0)
		return;
	D3D11_BUFFER_DESC desc = {};
	D3D11_SUBRESOURCE_DATA data = {};

	desc.ByteWidth = sizeof(ChunkVertex) * this->vertices.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = this->vertices.data();
	HRESULT hr = device->CreateBuffer(&desc, &data, this->vertex_buffer.GetAddressOf());
	CHECK(hr);
	desc.ByteWidth = sizeof(uint32) * this->indices.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	data.pSysMem = this->indices.data();
	hr = device->CreateBuffer(&desc, &data, this->index_buffer.GetAddressOf());
	CHECK(hr);
}

void	SubchunkMesh::draw(ComPtr<ID3D11DeviceContext> const& context) const
{
	if (this->vertex_buffer == nullptr || this->index_buffer == nullptr)
		return;
	uint32 stride = sizeof(ChunkVertex);
	uint32 offset = 0;

	context->IASetVertexBuffers(0, 1, this->vertex_buffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(this->indices.size(), 0, 0);
}