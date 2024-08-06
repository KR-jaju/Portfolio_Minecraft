#include "pch.h"
#include "DepthStencilView.h"

DepthStencilView::DepthStencilView(ComPtr<ID3D11Device> device, ID3D11Texture2D* texture)
	: device(device),
	view()
{
	ID3D11DepthStencilView** const	view = this->view.GetAddressOf();
	D3D11_TEXTURE2D_DESC				texture_desc = {};
	D3D11_DEPTH_STENCIL_VIEW_DESC		desc = {};

	texture->GetDesc(&texture_desc);
	desc.Format = texture_desc.Format;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = device->CreateDepthStencilView(texture, &desc, view);
	CHECK(hr);
}
