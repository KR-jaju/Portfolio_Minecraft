#include "pch.h"
#include "Chunk.h"
#include "Buffer.h"
#include "DeferredGraphics.h"

Chunk::Chunk()
{
	this->vertices_idx = 0;
	this->render_flag = false;
}

Chunk::~Chunk()
{
}

void Chunk::setGeoRender(
	ComPtr<ID3D11DeviceContext> const& context
)
{

	uint32 stride = this->geo_vbuffer->getStride();
	uint32 offset = this->geo_vbuffer->getOffset();
	context->IASetVertexBuffers(
		0,
		1,
		this->geo_vbuffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	context->Draw(this->geo_vbuffer->getCount(), 0);
}

void Chunk::setShadowRender(
	ComPtr<ID3D11DeviceContext> const& context
)
{
	uint32 stride = this->shadow_vbuffer->getStride();
	uint32 offset = this->shadow_vbuffer->getOffset();
	context->IASetVertexBuffers(
		0,
		1,
		this->shadow_vbuffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	context->IASetIndexBuffer(
		this->shadow_ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->DrawIndexed(
		this->shadow_ibuffer->getCount(),
		0,
		0
	);
}

void Chunk::createGeoBuffer(
	ComPtr<ID3D11Device> const& device,
	vector<VertexGeo> const& vertices
)
{
	this->geo_vbuffer = make_shared<Buffer<VertexGeo>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->render_flag = true;
}

void Chunk::createShadowBuffer(
	ComPtr<ID3D11Device> const& device,
	vector<VertexShadow> const& vertices,
	vector<uint32> const& indices
)
{
	this->shadow_vbuffer = make_shared<Buffer<VertexShadow>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->shadow_ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->render_flag = true;
}

void Chunk::reset()
{
	this->render_flag = false;
	this->vertices_idx = 0;
	this->max_h = -1;
	this->tp_block_cnt = 0;
	this->tp_chunk.reset();
}

void Chunk::setPos(Index2 const& c_pos)
{
	this->chunk_pos = c_pos;
	this->start_pos = vec3(c_pos.x + 0.5f, 0.5f, c_pos.y - 0.5f);
}

void Chunk::createGeoIBuffer(
	ComPtr<ID3D11Device> const& device, 
	vector<uint32> const& indices
)
{
	this->geo_ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
}

void Chunk::setGeoRenderIndices(
	ComPtr<ID3D11DeviceContext> const& context
)
{
	uint32 stride = this->geo_vbuffer->getStride();
	uint32 offset = this->geo_vbuffer->getOffset();
	context->IASetVertexBuffers(
		0,
		1,
		this->geo_vbuffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	context->IASetIndexBuffer(
		this->geo_ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->DrawIndexed(
		this->geo_ibuffer->getCount(),
		0, 0);
}

