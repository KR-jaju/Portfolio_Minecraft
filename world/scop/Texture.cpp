#include "pch.h"
#include "Texture.h"
#include "stb_image.h"
#include <directxtk/DDSTextureLoader.h> // 큐브맵 읽을 때 필요

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

void readImage(
	string const& path,
	vector<uint8_t>& image,
	int& width,
	int& height
)
{
	int channels;
	uint8_t* img = stbi_load(path.c_str(), &width, &height, 
		&channels, 0);
	image.resize(width * height * 4);
	if (channels == 3 || channels == 4) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t j = 0; j < 3; j++)
				image[4 * i + j] = img[i * channels + j];
			if (channels == 3)
				image[4 * i + 3] = 255;
			else
				image[4 * i + 3] = img[i * channels + 3];
		}
	}
	else if (channels == 1) {
		for (size_t i = 0; i < width * height; i++) {
			uint8_t g = img[i * channels + 0];
			for (size_t c = 0; c < 4; c++) {
				image[4 * i + c] = g;
			}
		}
	}
	else {
		cout << "Read 3 or 4 or 1 channels images only." << endl;
		exit(-1);
	}
	delete img;
}

ComPtr<ID3D11Texture2D> createStagingTexture(
	ComPtr<ID3D11Device>& device,
	ComPtr<ID3D11DeviceContext> context,
	int const width,
	int const height,
	vector<uint8_t> const& image
) 
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | 
		D3D11_CPU_ACCESS_WRITE;
	ComPtr<ID3D11Texture2D> staging_tex;

	HRESULT hr = device->
		CreateTexture2D(&desc, nullptr, staging_tex.GetAddressOf());
	CHECK(hr);

	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(staging_tex.Get(), 0, D3D11_MAP_WRITE, 0, &ms);
	uint8_t* pData = static_cast<uint8_t*>(ms.pData);
	for (UINT h = 0; h < UINT(height); h++) {
		memcpy(&pData[h * ms.RowPitch], &image[h * width * 4], 
			width * sizeof(uint8_t) * 4);
	}
	context->Unmap(staging_tex.Get(), 0);
	return staging_tex;
}

Texture::Texture(
	ComPtr<ID3D11Device>& device,
	ComPtr<ID3D11DeviceContext>& context,
	string const& path, 
	int mip_level
)
{
	int width, height;
	vector<uint8_t> image;

	readImage(path, image, width, height);
	ComPtr<ID3D11Texture2D> staging_texture =
		createStagingTexture(device, context, width, height, image);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.MipLevels = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
		D3D11_BIND_RENDER_TARGET;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.CPUAccessFlags = 0;
	ComPtr<ID3D11Texture2D> tex;
	HRESULT hr = device->CreateTexture2D(&desc, 
		nullptr, tex.GetAddressOf());
	CHECK(hr);
	context->CopySubresourceRegion(tex.Get(), 0, 0, 0, 0,
		staging_texture.Get(), 0, nullptr);

	hr = device->CreateShaderResourceView(tex.Get(), nullptr,
		this->sharder_resource_view.GetAddressOf());
	CHECK(hr);
	context->GenerateMips(this->sharder_resource_view.Get());
}

Texture::Texture(
	ComPtr<ID3D11Device>& device, 
	const wchar_t* filename, 
	bool is_cube_map
)
{
	ComPtr<ID3D11Texture2D> texture;
	UINT misc_flags = 0;
	if (is_cube_map)
		misc_flags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	HRESULT hr = CreateDDSTextureFromFileEx(
		device.Get(), filename, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, misc_flags, 
		DDS_LOADER_FLAGS(false),
		(ID3D11Resource **)texture.GetAddressOf(),
		this->sharder_resource_view.GetAddressOf(), NULL
	);
}

Texture::~Texture()
{
}

ComPtr<ID3D11ShaderResourceView> Texture::getComPtr() const
{
	return this->sharder_resource_view;
}
