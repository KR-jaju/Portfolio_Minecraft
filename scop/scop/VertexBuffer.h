#pragma once
class VertexBuffer
{
public:
	template<typename DATA>
	VertexBuffer(
		ComPtr<ID3D11Device> device,
		vector<DATA> const& vertices
	)
		: device(device)
	{
		this->stride = sizeof(DATA);
		this->count = static_cast<uint32>(vertices.size());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = this->stride * this->count;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();
		HRESULT hr = this->device->CreateBuffer(
			&desc,
			&data,
			this->buffer.GetAddressOf()
		);
		CHECK(hr);
	}
	~VertexBuffer();
	ComPtr<ID3D11Buffer> getComPtr() const;
	uint32 getStride() const;
	uint32 getCount() const;
	uint32 getOffset() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Buffer> buffer;
	uint32 stride = 0;
	uint32 offset = 0;
	uint32 count = 0;
};

