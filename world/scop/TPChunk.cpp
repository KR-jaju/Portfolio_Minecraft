#include "pch.h"
#include "TPChunk.h"
#include "Buffer.h"

TPChunk::TPChunk()
{
}

TPChunk::~TPChunk()
{
}

void TPChunk::createTPBuffer(ComPtr<ID3D11Device> const& device)
{
	this->tp_vbuffer = make_shared<Buffer<VertexColor>>(
		device,
		this->vertices.data(),
		this->vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->tp_ibuffer = make_shared<Buffer<uint32>>(
		device,
		this->indices.data(),
		this->indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
}

void TPChunk::setTPBuffer(
	ComPtr<ID3D11DeviceContext> const& context
)
{
	uint32 stride = this->tp_vbuffer->getStride();
	uint32 offset = this->tp_vbuffer->getOffset();
	context->IASetVertexBuffers(
		0, 1,
		this->tp_vbuffer->getComPtr().GetAddressOf(),
		&stride, &offset
	);
	context->IASetIndexBuffer(
		this->tp_ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->DrawIndexed(
		this->tp_ibuffer->getCount(),
		0, 0);
}

void TPChunk::update(ComPtr<ID3D11Device> const& device)
{
	if (this->vertices.size() == 0)
		return;
	this->tp_vbuffer = make_shared<Buffer<VertexColor>>(
		device,
		this->vertices.data(),
		this->vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->tp_ibuffer = make_shared<Buffer<uint32>>(
		device,
		this->indices.data(),
		this->indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->render_flag = true;
}

void TPChunk::reset()
{
	this->vertices_idx = 0;
	this->render_flag = false;
	this->vertices.clear();
	this->indices.clear();
}

