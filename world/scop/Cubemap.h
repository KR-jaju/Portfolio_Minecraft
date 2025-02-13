#pragma once

#include "Asset.h"
#include "Graphics.h"

class Cubemap : public Asset
{
public:
	static constexpr AssetType getAssetType()
	{
		return AssetType::Cubemap;
	}

	Cubemap(Graphics& graphics, wstring const& path);
	Cubemap(Cubemap const&) = delete;
	~Cubemap();
	Cubemap& operator=(Cubemap const&) = delete;
	ComPtr<ID3D11ShaderResourceView> getComPtr() const;
private:
	ComPtr<ID3D11ShaderResourceView> sharder_resource_view;
};

