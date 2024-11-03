#pragma once
class ComputeShader
{
public:
	ComputeShader(
		ComPtr<ID3D11Device> device,
		wstring const& path,
		string const& entry_point,
		string const& version
	);
	ComputeShader(ComputeShader const&) = delete;
	~ComputeShader();
	ComPtr<ID3D11ComputeShader> getComPtr() const;
	ComPtr<ID3DBlob> getBlob() const;

private:
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11ComputeShader> compute_shader;
};

