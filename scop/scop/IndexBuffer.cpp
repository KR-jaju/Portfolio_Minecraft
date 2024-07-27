#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(
	ComPtr<ID3D11Device> device, 
	vector<uint32> const& indices
)
	: device(device)
{
	D3D11_BUFFER_DESC desc;
	this->stride = sizeof(uint32);
	this->count = static_cast<uint32>(indices.size());
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = static_cast<uint32>(sizeof(uint32) * indices.size());

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data();
	HRESULT hr = this->device->CreateBuffer(
		&desc,
		&data,
		this->index_buffer.GetAddressOf()
	);
	CHECK(hr);
}

IndexBuffer::~IndexBuffer()
{
}

ComPtr<ID3D11Buffer> IndexBuffer::getComPtr() const
{
	return this->index_buffer;
}

uint32 IndexBuffer::getStride() const
{
	return this->stride;
}

uint32 IndexBuffer::getOffset() const
{
	return this->offset;
}

uint32 IndexBuffer::getCount() const
{
	return this->count;
}
