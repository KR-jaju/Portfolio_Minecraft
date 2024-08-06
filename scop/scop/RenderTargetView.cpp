#include "pch.h"
#include "RenderTargetView.h"

RenderTargetView::RenderTargetView(ComPtr<ID3D11Device> const& device, ID3D11Texture2D* texture)
	: device(device),
	view()
{
	ID3D11RenderTargetView** const	view = this->view.GetAddressOf();
	D3D11_TEXTURE2D_DESC				texture_desc = {};
	D3D11_RENDER_TARGET_VIEW_DESC		desc = {};

	texture->GetDesc(&texture_desc);
	desc.Format = texture_desc.Format;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = device->CreateRenderTargetView(texture, &desc, view);
	CHECK(hr);
}
