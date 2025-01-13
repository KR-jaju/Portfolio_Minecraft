#pragma once
class ConstantBuffer
{
public:
	template<typename Data>
	ConstantBuffer(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context,
		Data const& resource
	)
		: device(device), context(context)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Data);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = &resource;

		HRESULT hr = this->device->CreateBuffer(
			&desc,
			&data,
			this->buffer.GetAddressOf()
		);
		CHECK(hr);
	}
	ConstantBuffer(ConstantBuffer const&) = delete;
	~ConstantBuffer();
	ConstantBuffer& operator=(ConstantBuffer const&) = delete;
	ComPtr<ID3D11Buffer> getComPtr() const;

	template<typename Data>
	void update(const Data& resource) {
		D3D11_MAPPED_SUBRESOURCE subresource;
		ZeroMemory(&subresource, sizeof(subresource));
		HRESULT hr = this->context->Map(
			this->buffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&subresource
		);
		CHECK(hr);
		void* test_ptr = memcpy(subresource.pData, &resource, 
			sizeof(resource));
		if (!(test_ptr))
			cout << "constant buffer memcpy error" << endl;
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
