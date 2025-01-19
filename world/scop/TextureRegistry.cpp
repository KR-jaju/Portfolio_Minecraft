#include "pch.h"
#include "TextureRegistry.h"

TextureRegistry::TextureRegistry(Graphics& graphics)
	: block_texture_array(graphics.getDevice(), graphics.getContext(), {
		L"grass_top.png",
		L"grass_bottom.png",
		L"grass_side.png"
	})
{
	//this->block_textures.emplace({0, 0, 0, 0, 0, 0});
	this->block_textures[BlockData(1, 0)] = { 0, 0, 0, 0, 0, 0 };
}

BlockTextureData const& TextureRegistry::getBlockTextureData(BlockData data) const
{
	return (this->block_textures.at(data));
}

ComPtr<ID3D11ShaderResourceView> TextureRegistry::getBlockTextureArray()
{
	return (this->block_texture_array.getComPtr());
}