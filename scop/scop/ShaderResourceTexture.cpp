#include "pch.h"
#include "ShaderResourceTexture.h"

ShaderResourceTexture::ShaderResourceTexture(Context const& context, uint32 width, uint32 height)
	: device(context.getDevice()),
	device_context(context.getDeviceContext()),
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
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT	hr = device->CreateTexture2D(&desc, nullptr, this->texture.GetAddressOf());
	CHECK(hr);
}

void	ShaderResourceTexture::setPixels(void const* pixels, uint32 stride, uint32 x, uint32 y, uint32 w, uint32 h)
{
	ID3D11Texture2D* internal_texture = this->getInternalResource();
	D3D11_BOX	box = {
		x, y, 0, x + w, y + h, 1
	};

	this->device_context->UpdateSubresource(internal_texture, 0, &box, pixels, stride, 0);
	
}

ID3D11Texture2D*	ShaderResourceTexture::getInternalResource() const
{
	return (this->texture.Get());
}


