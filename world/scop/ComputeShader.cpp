#include "pch.h"
#include "ComputeShader.h"

ComputeShader::ComputeShader(
	ComPtr<ID3D11Device> device, 
	wstring const& path, 
	string const& entry_point, 
	string const& version
)
{
	uint32 compile_flag = D3DCOMPILE_DEBUG |
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

	hr = device->CreateComputeShader(
		this->blob->GetBufferPointer(),
		this->blob->GetBufferSize(),
		nullptr,
		this->compute_shader.GetAddressOf()
	);
	CHECK(hr);
}

ComputeShader::~ComputeShader()
{
}

ComPtr<ID3D11ComputeShader> ComputeShader::getComPtr() const
{
	return this->compute_shader;
}

ComPtr<ID3DBlob> ComputeShader::getBlob() const
{
	return this->blob;
}
