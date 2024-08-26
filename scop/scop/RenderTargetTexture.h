#pragma once

#include "Texture2D.h"

class RenderTargetTexture : public Texture2D
{
public:
	RenderTargetTexture(Context const& context, uint32 width, uint32 height);
	ID3D11Texture2D* getInternalResource() const;
private:
	ID3D11Device* device;
	ComPtr<ID3D11Texture2D>	texture;
};