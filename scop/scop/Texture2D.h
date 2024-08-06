#pragma once

#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "RenderTargetView.h"

class Texture2D {
public:
	Texture2D(ComPtr<ID3D11Device> const& device, uint32 width, uint32 height);
	DepthStencilView	getDepthStencilView() const;
	ShaderResourceView	getShaderResourceView() const;
	RenderTargetView	getRenderTargetView() const;
private:
	ComPtr<ID3D11Device>	device;
	ComPtr<ID3D11Texture2D>	texture;
};
