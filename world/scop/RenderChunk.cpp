#include "pch.h"
#include "RenderChunk.h"

RenderChunk::RenderChunk(int chunk_x, int chunk_z)
	: chunk_x(chunk_x), chunk_z(chunk_z), is_synced(false)
{

}

void	RenderChunk::setMesh(std::vector<ChunkVertex>&& vertices, std::vector<uint32>&& indices)
{
	this->vertices = std::move(vertices);
	this->indices = std::move(indices);
}

void	RenderChunk::draw(Graphics& graphics) const
{
	ComPtr<ID3D11Device> device = graphics.getDevice();
	ComPtr<ID3D11DeviceContext> const dc = graphics.getContext();

	if (this->vertices.size() == 0)
		return;
	if (this->mvp_constant == nullptr)
		this->mvp_constant = std::make_unique<ConstantBuffer>(device, dc, this->mvp);
	if (!this->is_synced)
	{
		this->vertex_buffer = std::make_unique<Buffer<ChunkVertex>>(device, this->vertices.data(), this->vertices.size(), D3D11_BIND_VERTEX_BUFFER);
		this->index_buffer = std::make_unique<Buffer<uint32>>(device, this->indices.data(), this->indices.size(), D3D11_BIND_INDEX_BUFFER);
		this->is_synced = true;
	}
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();

	dc->IASetVertexBuffers(
		0,
		1,
		this->vertex_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	dc->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	dc->DrawIndexed(
		this->index_buffer->getCount(),
		0,
		0
	);
}