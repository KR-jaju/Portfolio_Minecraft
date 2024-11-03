#pragma once
class DomainShader
{
public:
	DomainShader(
		ComPtr<ID3D11Device> device,
		wstring const& path,
		string const& entry_point,
		string const& version
	);
	DomainShader(DomainShader const&) = delete;
	DomainShader& operator=(DomainShader const&) = delete;
	~DomainShader();
	ComPtr<ID3D11DomainShader> getComPtr() const;
	ComPtr<ID3DBlob> getBlob() const;

private:
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11DomainShader> domain_shader;
};

