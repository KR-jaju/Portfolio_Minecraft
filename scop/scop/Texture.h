#pragma once
class Texture
{
public:
	Texture(
		ComPtr<ID3D11Device> device,
		wstring const& path
	);
	Texture(Texture const&) = delete;
	~Texture();
	Texture& operator=(Texture const&) = delete;
	ComPtr<ID3D11ShaderResourceView> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11ShaderResourceView> sharder_resource_view;
};

