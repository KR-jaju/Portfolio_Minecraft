#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(ComPtr<ID3D11Device> device)
	: device(device)
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::create(
	const vector<D3D11_INPUT_ELEMENT_DESC>& descs, 
	ComPtr<ID3DBlob> vertexShaderBlob
)
{
	uint32 size = static_cast<uint32>(descs.size());
	HRESULT hr = this->device->CreateInputLayout(
		descs.data(),
		size,
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		this->input_layout.GetAddressOf()
	);
	CHECK(hr);
}

ComPtr<ID3D11InputLayout> InputLayout::getComPtr() const
{
	return this->input_layout;
}
