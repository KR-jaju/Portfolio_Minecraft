#include "pch.h"
#include "DepthMap.h"

DepthMap::DepthMap(
	ComPtr<ID3D11Device> const& device, 
	UINT width, 
	UINT height
)
{
	this->width = width;
	this->height = height;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = this->width;
	desc.Height = this->height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_SHADER_RESOURCE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	HRESULT hr = device->CreateTexture2D(&desc, NULL, 
		this->depth_texture.GetAddressOf());
	CHECK(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));
	dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(this->depth_texture.Get(), 
		&dsv_desc, this->depth_stencil_view.GetAddressOf());
	CHECK(hr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(srv_desc));
	srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(this->depth_texture.Get(),
		&srv_desc, this->depth_SRV.GetAddressOf());
	CHECK(hr);
}

ComPtr<ID3D11DepthStencilView> DepthMap::getDepthStencilView()
{
	return this->depth_stencil_view;
}

ComPtr<ID3D11ShaderResourceView> DepthMap::getShaderResourceView()
{
	return this->depth_SRV;
}

ComPtr<ID3D11Texture2D> DepthMap::getTexture2D()
{
	return this->depth_texture;
}

D3D11_VIEWPORT DepthMap::getViewPort()
{
	return this->view_port;
}

void DepthMap::setViewPort(float width, float height)
{
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = width;
	this->view_port.Height = height;
	this->view_port.MinDepth = 0.0f;
	this->view_port.MaxDepth = 1.0f;
}
