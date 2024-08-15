#include "pch.h"
#include "ShaderResourceView.h"

ShaderResourceView::ShaderResourceView(Context const& context, ShaderResourceTexture const& texture)
	: device(context.getDevice()),
	view()
{
	ID3D11Texture2D* const	internal_texture = texture.getInternalResource();
	ID3D11ShaderResourceView** const	view = this->view.GetAddressOf();
	D3D11_TEXTURE2D_DESC				texture_desc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC		desc = {};

	internal_texture->GetDesc(&texture_desc);
	desc.Format = texture_desc.Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;

	HRESULT hr = device->CreateShaderResourceView(internal_texture, &desc, view);
	CHECK(hr);
}
