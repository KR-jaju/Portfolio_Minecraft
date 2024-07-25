#include "pch.h"
#include "PixelShader.h"



PixelShader::PixelShader(ComPtr<ID3D11Device> device)
	: device(device)
{
}

PixelShader::~PixelShader()
{
}

ComPtr<ID3D11PixelShader> PixelShader::getComPtr() const
{
	return this->pixel_shader;
}

void PixelShader::create(
	const wstring& path, 
	const string& name, 
	const string& version)
{
	loadShaderFromFile(path, name, version);
	HRESULT hr = this->device->CreatePixelShader(
		this->blob->GetBufferPointer(),
		this->blob->GetBufferSize(),
		nullptr,
		this->pixel_shader.GetAddressOf()
	);
	CHECK(hr);
}
