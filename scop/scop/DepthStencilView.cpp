#include "pch.h"
#include "DepthStencilView.h"

DepthStencilView::DepthStencilView(Context const& context, DepthStencilTexture const& texture)
	: device(context.getDevice()),
	view()
{
	ID3D11Texture2D* const	internal_texture = texture.getInternalResource();
	ID3D11DepthStencilView** const	view = this->view.GetAddressOf();
	D3D11_TEXTURE2D_DESC				texture_desc = {};
	D3D11_DEPTH_STENCIL_VIEW_DESC		desc = {};

	internal_texture->GetDesc(&texture_desc);
	desc.Format = texture_desc.Format;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = device->CreateDepthStencilView(internal_texture, &desc, view);
	CHECK(hr);
}

ID3D11DepthStencilView* DepthStencilView::getInternalResource() const
{
	return (this->view.Get());
}
