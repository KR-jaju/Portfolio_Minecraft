#pragma once

#include "Texture2D.h"

class ShaderResourceTexture : public Texture2D
{
public:
	ShaderResourceTexture(Context const& context, uint32 width, uint32 height);
	void	setPixels(void const*pixels, uint32 stride, uint32 x, uint32 y, uint32 w, uint32 h);
	ID3D11Texture2D* getInternalResource() const;
private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	ComPtr<ID3D11Texture2D>	texture;
};
