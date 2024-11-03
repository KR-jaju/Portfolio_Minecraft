#include "pch.h"
#include "DomainShader.h"

DomainShader::DomainShader(
	ComPtr<ID3D11Device> device, 
	wstring const& path, 
	string const& entry_point, 
	string const& version
)
{
	const uint32 compile_flag = D3DCOMPILE_DEBUG |
		D3DCOMPILE_SKIP_OPTIMIZATION;
	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry_point.c_str(),
		version.c_str(),
		compile_flag,
		0,
		this->blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);

	hr = device->CreateDomainShader(
		this->blob->GetBufferPointer(),
		this->blob->GetBufferSize(),
		nullptr,
		this->domain_shader.GetAddressOf()
	);
	CHECK(hr);
}

DomainShader::~DomainShader()
{
}

ComPtr<ID3D11DomainShader> DomainShader::getComPtr() const
{
	return this->domain_shader;
}

ComPtr<ID3DBlob> DomainShader::getBlob() const
{
	return this->blob;
}
