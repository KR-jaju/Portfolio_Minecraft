#include "pch.h"
#include "Graphics.h"
#include "SubchunkMeshData.h"

SubchunkMeshData::SubchunkMeshData()
	: vertices(), indices(), mutex(), is_buffer_synced(true)
{}

void	SubchunkMeshData::updateMesh(std::vector<ChunkVertex>&& vertices, std::vector<uint32>&& indices)
{
	std::lock_guard<std::mutex> lock(this->mutex);

	this->is_buffer_synced = (indices.size() == 0);
	this->vertices = std::move(vertices);
	this->indices = std::move(indices);
}

void	SubchunkMeshData::draw(Graphics& graphics)
{
	{
		std::lock_guard<std::mutex> lock(this->mutex);

		if (!this->is_buffer_synced)
		{
			this->vertex_buffer = make_unique<Buffer<ChunkVertex>>(graphics.getDevice(), this->vertices.data(), this->vertices.size(), D3D11_BIND_VERTEX_BUFFER);
			this->index_buffer = make_unique<Buffer<uint32>>(graphics.getDevice(), this->indices.data(), this->indices.size(), D3D11_BIND_INDEX_BUFFER);
			this->is_buffer_synced = true;
		}
	}
	if (this->vertex_buffer == nullptr)
		return;
	ComPtr<ID3D11DeviceContext> context = graphics.getContext();
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();

	context->IASetVertexBuffers(0, 1, this->vertex_buffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->index_buffer->getComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(this->index_buffer->getCount(), 0, 0);
}
