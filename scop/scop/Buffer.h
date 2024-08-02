#pragma once


template<typename Data>
class Buffer
{
public:
	Buffer(
		ComPtr<ID3D11Device> device,
		Data const* data_array,
		uint32 count,
		UINT bind_flag
	)
		: device(device)
	{
		this->stride = sizeof(Data);
		this->count = count;
		
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = this->stride * this->count;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = bind_flag;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = data_array;
		HRESULT hr = this->device->CreateBuffer(
			&desc,
			&data,
			this->buffer.GetAddressOf()
		);
		CHECK(hr);
	}
	Buffer(Buffer const&) = delete;
	~Buffer() {};
	Buffer& operator=(Buffer const&) = delete;
	ComPtr<ID3D11Buffer> getComPtr() const { return this->buffer; }
	uint32 getStride() const { return this->stride; }
	uint32 getOffset() const { return this->offset; }
	uint32 getCount() const { return this->count; }
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Buffer> buffer;
	uint32 stride = 0;
	uint32 offset = 0;
	uint32 count = 0;
};