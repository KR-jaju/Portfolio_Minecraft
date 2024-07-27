#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::~VertexBuffer()
{
}

ComPtr<ID3D11Buffer> VertexBuffer::getComPtr() const
{
	return this->buffer;
}

uint32 VertexBuffer::getStride() const
{
	return this->stride;
}

uint32 VertexBuffer::getCount() const
{
	return this->count;
}

// offset 을 0 말고 다른 걸로 주는 경우가 있는 지 잘 모르겠음
uint32 VertexBuffer::getOffset() const
{
	return this->offset;
}

