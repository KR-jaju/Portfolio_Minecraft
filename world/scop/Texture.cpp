#include "pch.h"
#include "Texture.h"

Texture::Texture(
	ComPtr<ID3D11Device> device, 
	wstring const& path
)
	: device(device)
{
	TexMetadata meta_data;
	ScratchImage img;
	HRESULT hr = LoadFromWICFile(
		path.c_str(),
		WIC_FLAGS_NONE,
		&meta_data,
		img
	);
	CHECK(hr);

	hr = CreateShaderResourceView(
		this->device.Get(),
		img.GetImages(),
		img.GetImageCount(),
		meta_data,
		this->sharder_resource_view.GetAddressOf()
	);

	CHECK(hr);
}

Texture::~Texture()
{
}

ComPtr<ID3D11ShaderResourceView> Texture::getComPtr() const
{
	return this->sharder_resource_view;
}
