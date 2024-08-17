#include "pch.h"
#include "RenderTargetView.h"
#include "SwapChain.h"

RenderTargetView::RenderTargetView(Context const& context, RenderTargetTexture const& texture)
	: device(context.getDevice()),
	device_context(context.getDeviceContext()),
	view()
{
	ID3D11Texture2D* const	internal_texture = texture.getInternalResource();
	ID3D11RenderTargetView** const	view = this->view.GetAddressOf();
	D3D11_TEXTURE2D_DESC				texture_desc = {};
	D3D11_RENDER_TARGET_VIEW_DESC		desc = {};

	internal_texture->GetDesc(&texture_desc);
	desc.Format = texture_desc.Format;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = device->CreateRenderTargetView(internal_texture, &desc, view);
	CHECK(hr);
}

RenderTargetView::RenderTargetView(Context const& context, SwapChain const& swap_chain)
	: device(context.getDevice()),
	device_context(context.getDeviceContext()),
	view()
{
	ID3D11Texture2D* const	internal_texture = swap_chain.getBackBuffer();
	ID3D11RenderTargetView** const	view = this->view.GetAddressOf();
	D3D11_TEXTURE2D_DESC				texture_desc = {};
	D3D11_RENDER_TARGET_VIEW_DESC		desc = {};

	internal_texture->GetDesc(&texture_desc);
	desc.Format = texture_desc.Format;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = this->device->CreateRenderTargetView(internal_texture, nullptr, view);
	CHECK(hr);
}

void RenderTargetView::clear(float r, float g, float b, float a)
{
	ID3D11RenderTargetView* const internal_view = this->getInternalResource();
	float const color[4] = { r, g, b, a };

	this->device_context->ClearRenderTargetView(internal_view, color);
}

ID3D11RenderTargetView* RenderTargetView::getInternalResource() const
{
	return (this->view.Get());
}

ID3D11RenderTargetView*const* RenderTargetView::getAddressOf() const
{
	return (this->view.GetAddressOf());
}
