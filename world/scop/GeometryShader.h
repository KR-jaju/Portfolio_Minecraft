#pragma once
class GeometryShader
{
public:
	GeometryShader(
		ComPtr<ID3D11Device> device,
		wstring const& path,
		string const& entry_point,
		string const& version
	);
	~GeometryShader();
	GeometryShader(GeometryShader const&) = delete;
	GeometryShader& operator=(GeometryShader const&) = delete;
	ComPtr<ID3D11GeometryShader> getComPtr() const;
	ComPtr<ID3DBlob> getBlob() const;

private:
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11GeometryShader> geo_shader;
};

