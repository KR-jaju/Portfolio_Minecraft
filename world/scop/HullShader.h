#pragma once
class HullShader
{
public:
	HullShader(
		ComPtr<ID3D11Device> device,
		wstring const& path,
		string const& entry_point,
		string const& version
	);
	~HullShader();
	HullShader(HullShader const&) = delete;
	HullShader& operator=(HullShader const&) const = delete;
	ComPtr<ID3D11HullShader> getComPtr() const;
	ComPtr<ID3DBlob> getBlob() const;

private:
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11HullShader> hull_shader;
};

