#include "pch.h"
#include "RenderTargetTexture.h"

RenderTargetTexture::RenderTargetTexture(Context const& context, uint32 width, uint32 height)
	: device(context.getDevice()),
	texture()
{
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = static_cast<UINT>(width);
	desc.Height = static_cast<UINT>(height);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		//D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT	hr = device->CreateTexture2D(&desc, nullptr, this->texture.GetAddressOf());
	CHECK(hr);
}

ID3D11Texture2D* RenderTargetTexture::getInternalResource() const
{
	return (this->texture.Get());
}
