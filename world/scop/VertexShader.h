#pragma once

class VertexShader
{
public:
	VertexShader(
		ComPtr<ID3D11Device> device,
		wstring const& path,
		string const& entry_point,
		string const& version
	);
	virtual ~VertexShader();
	ComPtr<ID3D11VertexShader> getComPtr() const;
	ComPtr<ID3DBlob> getBlob() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11VertexShader> vertex_shader;
};

