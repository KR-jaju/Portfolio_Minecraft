#pragma once

class PixelShader
{
public:
	PixelShader(
		ComPtr<ID3D11Device> device,
		wstring const& path,
		string const& entry_point,
		string const& version
	);
	PixelShader(PixelShader const&) = delete;
	virtual ~PixelShader();
	PixelShader& operator=(PixelShader const&) const = delete;
	ComPtr<ID3D11PixelShader> getComPtr() const;
	ComPtr<ID3DBlob> getBlob() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11PixelShader> pixel_shader;
};

