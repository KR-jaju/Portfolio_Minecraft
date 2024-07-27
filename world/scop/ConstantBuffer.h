#pragma once
class ConstantBuffer
{
public:
	template<typename DATA>
	ConstantBuffer(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> deviceContext,
		DATA const& resource
	)
		: device(device), context(deviceContext)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(DATA);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = &resource;


		HRESULT hr = this->device->CreateBuffer(
			&desc,
			&data,
			this->buffer.GetAddressOf()
		);
		CHECK(hr);
	}
	~ConstantBuffer();
	ComPtr<ID3D11Buffer> getComPtr() const;

	template<typename DATA>
	void update(const DATA& resource) {
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));
		this->context->Map(
			this->buffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&subResource
		);
		memcpy(subResource.pData, &resource, sizeof(resource));
		this->context->Unmap(
			this->buffer.Get(),
			0
		);
	}
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<ID3D11Buffer> buffer;
};
