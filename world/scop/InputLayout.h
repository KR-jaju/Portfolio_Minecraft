#pragma once
class InputLayout
{
public:
	InputLayout(
		ComPtr<ID3D11Device> device,
		vector<D3D11_INPUT_ELEMENT_DESC> const& descs,
		ComPtr<ID3DBlob> vertexShaderBlob
	);
	~InputLayout();
	ComPtr<ID3D11InputLayout> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11InputLayout> input_layout;
};

