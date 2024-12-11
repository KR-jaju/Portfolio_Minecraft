#pragma once
class TextureArray
{
public:
	TextureArray(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context,
		vector<wstring> const& path_arr
	);
	TextureArray(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context,
		vector<wstring> const& path_arr,
		int mip_level,
		bool srgb_flag = false
	);
	TextureArray(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context,
		vector<ComPtr<ID3D11ShaderResourceView>> const& srvs,
		DXGI_FORMAT dxgi_format
	);
	void updateTextureArray(
		ComPtr<ID3D11DeviceContext> context,
		ComPtr<ID3D11ShaderResourceView> target_srv,
		UINT arr_idx
	);
	ComPtr<ID3D11ShaderResourceView> getComPtr();
private:
	ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	ComPtr<ID3D11Texture2D> texture_arr;
};

