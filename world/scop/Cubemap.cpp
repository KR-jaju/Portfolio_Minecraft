#include "pch.h"
#include "Cubemap.h"
#include <directxtk/DDSTextureLoader.h>

Cubemap::Cubemap(Graphics& graphics, wstring const& path)
{
	ComPtr<ID3D11Device> device = graphics.getDevice();
	ComPtr<ID3D11Texture2D> texture;

	HRESULT hr = CreateDDSTextureFromFileEx(
		device.Get(), path.c_str(), 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE,
		DDS_LOADER_FLAGS(false),
		(ID3D11Resource**)texture.GetAddressOf(),
		this->sharder_resource_view.GetAddressOf(), nullptr
	);
	CHECK(hr);
}

Cubemap::~Cubemap()
{
}

ComPtr<ID3D11ShaderResourceView> Cubemap::getComPtr() const
{
	return this->sharder_resource_view;
}
