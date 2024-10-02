#pragma once
//desc의 정보를 받아 쉐이더에 넘겨줄 정보를 만드는 InputLayout을 만드는 클래스이다.
// desc의 size정보도 같이 인자값으로 넘겨서 준다.
class InputLayout
{
public:
	InputLayout(
		ComPtr<ID3D11Device> device,
		D3D11_INPUT_ELEMENT_DESC const* descs,
		uint32 count,
		ComPtr<ID3DBlob> vertexShaderBlob
	);
	InputLayout(InputLayout const&) = delete;
	~InputLayout();
	InputLayout& operator=(InputLayout const&) = delete;
	ComPtr<ID3D11InputLayout> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11InputLayout> input_layout;
};

