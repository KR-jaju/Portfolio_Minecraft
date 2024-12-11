#include "pch.h"
#include "GeometryShader.h"

GeometryShader::GeometryShader(
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
	hr = device->CreateGeometryShader(
		this->blob->GetBufferPointer(),
		this->blob->GetBufferSize(),
		nullptr,
		this->geo_shader.GetAddressOf()
	);
	CHECK(hr);
}

GeometryShader::~GeometryShader()
{
}

ComPtr<ID3D11GeometryShader> GeometryShader::getComPtr() const
{
	return this->geo_shader;
}

ComPtr<ID3DBlob> GeometryShader::getBlob() const
{
	return this->blob;
}
