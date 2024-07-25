#pragma once
class Texture
{
public:
	Texture(ComPtr<ID3D11Device> device);
	~Texture();
	void create(const wstring& path);
	ComPtr<ID3D11ShaderResourceView> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11ShaderResourceView> sharder_resource_view;
};

