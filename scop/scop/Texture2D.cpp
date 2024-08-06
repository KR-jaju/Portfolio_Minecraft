#include "pch.h"
#include "Texture2D.h"

Texture2D::Texture2D(ComPtr<ID3D11Device> const& device, uint32 width, uint32 height)
	: texture()
{
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = static_cast<UINT>(width);
	textureDesc.Height = static_cast<UINT>(height);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT	hr = device->CreateTexture2D(&textureDesc, nullptr, this->texture.GetAddressOf());
	CHECK(hr);
}


DepthStencilView	Texture2D::getDepthStencilView() const
{
	return DepthStencilView(this->device, this->texture.Get());
}

RenderTargetView	Texture2D::getRenderTargetView() const
{
	return RenderTargetView(this->device, this->texture.Get());
}

ShaderResourceView	Texture2D::getShaderResourceView() const
{
	return ShaderResourceView(this->device, this->texture.Get());
}
