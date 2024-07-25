#include "pch.h"
#include "VertexShader.h"


VertexShader::VertexShader(ComPtr<ID3D11Device> device) 
 : device(device)
{
}

VertexShader::~VertexShader()
{
}

ComPtr<ID3D11VertexShader> VertexShader::getComPtr() const
{
	return this->vertex_shader;
}



void VertexShader::create(
	const wstring& path, 
	const string& name, 
	const string& version
)
{
	loadShaderFromFile(path, name, version);
	HRESULT hr = this->device->CreateVertexShader(
		this->blob->GetBufferPointer(),
		this->blob->GetBufferSize(),
		nullptr,
		this->vertex_shader.GetAddressOf()
	);
	CHECK(hr);
}

