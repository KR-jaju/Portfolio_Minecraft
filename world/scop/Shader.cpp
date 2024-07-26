#include "pch.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

ComPtr<ID3DBlob> Shader::getBlob() const
{
	return this->blob;
}

void Shader::loadShaderFromFile(
	const wstring& path, 
	const string& name, 
	const string& version
)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG 
		| D3DCOMPILE_SKIP_OPTIMIZATION;
	
	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		this->blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);
}

