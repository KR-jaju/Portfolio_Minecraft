#pragma once

enum class AssetType
{
	Texture2D,
	TextureArray,
	Cubemap,
	AnimationClip,
	SkinnedMesh,
	Blob
};

class Asset
{
public:
	virtual ~Asset() = default;
};
