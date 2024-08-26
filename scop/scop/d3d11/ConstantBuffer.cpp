#include "pch.h"
#include "ConstantBuffer.h"
#include "Device.h"

namespace d3d11
{
	ConstantBuffer::ConstantBuffer(Device const& device, uint32 byte_size)
		: buffer(),
		size(byte_size)
	{
		ID3D11Device* internal_device = device.getDevice();
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = byte_size;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = internal_device->CreateBuffer(
			&desc,
			nullptr,
			this->buffer.GetAddressOf()
		);
		CHECK(hr);
	}

	uint32 ConstantBuffer::getByteSize() const
	{
		return (this->size);
	}

	//void	ConstantBuffer::update(void* data, uint32 size, uint32 offset) {
	//	ID3D11Buffer* const buffer = this->buffer.Get();
	//	D3D11_MAPPED_SUBRESOURCE subresource = {};

	//	this->context->Map(
	//		buffer,
	//		0,
	//		D3D11_MAP_WRITE_DISCARD,
	//		0,
	//		&subresource
	//	);
	//	memcpy(subresource.pData, &resource, sizeof(resource));
	//	this->context->Unmap(buffer, 0);
	//}

	ID3D11Buffer* ConstantBuffer::getInternalResource() const
	{
		return (this->buffer.Get());
	}
}