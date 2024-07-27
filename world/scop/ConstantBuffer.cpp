#include "pch.h"
#include "ConstantBuffer.h"


ConstantBuffer::~ConstantBuffer()
{
}

ComPtr<ID3D11Buffer> ConstantBuffer::getComPtr() const
{
	return this->buffer;
}

