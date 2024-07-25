#pragma once
class InputLayout
{
public:
	InputLayout(ComPtr<ID3D11Device> device);
	~InputLayout();
	void create(
		const vector<D3D11_INPUT_ELEMENT_DESC>& descs,
		ComPtr<ID3DBlob> vertexShaderBlob
	);
	ComPtr<ID3D11InputLayout> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11InputLayout> input_layout;
};

