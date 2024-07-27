#include "pch.h"
#include "VertexShader.h"


VertexShader::VertexShader(
	ComPtr<ID3D11Device> device, 
	wstring const& path, 
	string const& entry_point, 
	string const& version
)
	: device(device)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG
		| D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry_point.c_str(),
		version.c_str(),
		compileFlag,
		0,
		this->blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);
	
	hr = this->device->CreateVertexShader(
		this->blob->GetBufferPointer(),
		this->blob->GetBufferSize(),
		nullptr,
		this->vertex_shader.GetAddressOf()
	);
	CHECK(hr);
}

VertexShader::~VertexShader()
{
}

ComPtr<ID3D11VertexShader> VertexShader::getComPtr() const
{
	return this->vertex_shader;
}

ComPtr<ID3DBlob> VertexShader::getBlob() const
{
	return this->blob;
}

