#pragma once

class Context;

class InputLayout
{
public:
	InputLayout(
		Context const& context,
		D3D11_INPUT_ELEMENT_DESC const* descs,
		uint32 count,
		ComPtr<ID3DBlob> vertexShaderBlob
	);
	InputLayout(InputLayout const&) = delete;
	~InputLayout();
	InputLayout& operator=(InputLayout const&) = delete;
	ComPtr<ID3D11InputLayout> getComPtr() const;
private:
	ID3D11Device* device;
	ComPtr<ID3D11InputLayout> input_layout;
};

