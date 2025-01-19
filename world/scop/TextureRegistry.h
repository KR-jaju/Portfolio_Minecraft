#pragma once

#include "Graphics.h"
#include <map>
#include "BlockTextureData.h"
#include "BlockData.h"
#include "TextureArray.h"

class TextureRegistry
{
public:
	TextureRegistry(Graphics& graphics);
	BlockTextureData const& getBlockTextureData(BlockData data) const;
	ComPtr<ID3D11ShaderResourceView> getBlockTextureArray();
private:
	std::map<BlockData, BlockTextureData> block_textures;
	TextureArray block_texture_array;
};
