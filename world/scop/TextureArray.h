#pragma once

#include "Graphics.h"
#include "Asset.h"

class TextureArray : public Asset
{
public:
	static constexpr AssetType getAssetType()
	{
		return AssetType::TextureArray;
	}

	TextureArray(Graphics& graphics, std::wstring const& path);
	TextureArray(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context,
		vector<wstring> const& path_arr,
		int mip_level
	);
	ComPtr<ID3D11ShaderResourceView> getComPtr();
private:
	ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	ComPtr<ID3D11Texture2D> texture_arr;
};

