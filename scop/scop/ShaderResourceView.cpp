#include "pch.h"
#include "ShaderResourceView.h"

ShaderResourceView::ShaderResourceView(ComPtr<ID3D11Device> const& device, ID3D11Texture2D*	texture)
	: device(device),
	view()
{
	ID3D11ShaderResourceView** const	view = this->view.GetAddressOf();
	D3D11_TEXTURE2D_DESC				texture_desc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC		desc = {};

	texture->GetDesc(&texture_desc);
	desc.Format = texture_desc.Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;

	HRESULT hr = device->CreateShaderResourceView(texture, &desc, view);
	CHECK(hr);
}
