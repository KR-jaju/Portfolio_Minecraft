#include "pch.h"
#include "TextureResource.h"

TextureResource::TextureResource(ComPtr<ID3D11Device> device)
	:Super(ResourceType::Texture), device(device)
{
}

TextureResource::~TextureResource()
{
}

void TextureResource::Create(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	CHECK(hr);
	
	hr = ::CreateShaderResourceView(this->device.Get(), img.GetImages(), img.GetImageCount(), md, this->shader_resource_view.GetAddressOf());
	CHECK(hr);
	this->size.x = md.width;
	this->size.y = md.height;


}
