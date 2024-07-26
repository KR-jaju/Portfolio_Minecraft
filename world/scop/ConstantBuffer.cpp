#include "pch.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(
	ComPtr<ID3D11Device> device, 
	ComPtr<ID3D11DeviceContext> deviceContext
) : device(device), context(deviceContext)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

ComPtr<ID3D11Buffer> ConstantBuffer::getComPtr() const
{
	return this->buffer;
}

