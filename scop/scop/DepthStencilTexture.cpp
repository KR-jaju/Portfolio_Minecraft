#include "pch.h"
#include "DepthStencilTexture.h"

DepthStencilTexture::DepthStencilTexture(Context const& context, uint32 width, uint32 height)
	: device(context.getDevice()),
	texture()
{
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = static_cast<UINT>(width);
	desc.Height = static_cast<UINT>(height);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	//desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT	hr = device->CreateTexture2D(&desc, nullptr, this->texture.GetAddressOf());
	CHECK(hr);
}

ID3D11Texture2D* DepthStencilTexture::getInternalResource() const
{
	return (this->texture.Get());
}