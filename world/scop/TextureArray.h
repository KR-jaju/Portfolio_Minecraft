#pragma once
class TextureArray
{
public:
	TextureArray(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context,
		vector<wstring> const& path_arr
	);
	ComPtr<ID3D11ShaderResourceView> getComPtr();
private:
	ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	ComPtr<ID3D11Texture2D> texture_arr;
};

