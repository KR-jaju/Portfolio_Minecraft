#include "pch.h"
#include "InputLayout.h"
#include "Context.h"


InputLayout::InputLayout(
	Context const& context,
	D3D11_INPUT_ELEMENT_DESC const* descs, 
	uint32 count,
	ComPtr<ID3DBlob> vertexShaderBlob
)
	: device(context.getDevice())
{
	HRESULT hr = this->device->CreateInputLayout(
		descs,
		count,
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		this->input_layout.GetAddressOf()
	);
	CHECK(hr);
}

InputLayout::~InputLayout()
{
}


ComPtr<ID3D11InputLayout> InputLayout::getComPtr() const
{
	return this->input_layout;
}
