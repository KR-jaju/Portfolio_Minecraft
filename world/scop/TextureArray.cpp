#include "pch.h"
#include "TextureArray.h"

TextureArray::TextureArray(
	ComPtr<ID3D11Device> device, 
	ComPtr<ID3D11DeviceContext> context,
	vector<wstring> const& path_arr
)
{
	D3D11_TEXTURE2D_DESC texture_desc;
	vector<ID3D11Texture2D*> textures(path_arr.size());
	for (int i = 0; i < path_arr.size(); i++) {
		ID3D11Resource* resource = nullptr;
		HRESULT hr = CreateWICTextureFromFile(
			device.Get(),
			context.Get(),
			path_arr[i].c_str(),
			&resource,
			nullptr
		); // img 파일 읽어서 texture resource 변환
		CHECK(hr);
		textures[i] = static_cast<ID3D11Texture2D*>(resource);
	
		if (i == 0) {
			textures[i]->GetDesc(&texture_desc);
		}
		else {
			D3D11_TEXTURE2D_DESC desc;
			textures[i]->GetDesc(&desc);

			// 모든 텍스처가 동일한 메타데이터를 가지는지 확인
			if (desc.Width != texture_desc.Width ||
				desc.Height != texture_desc.Height ||
				desc.Format != texture_desc.Format ||
				desc.MipLevels != texture_desc.MipLevels) {
				throw std::runtime_error("All textures must have the same dimensions, format, and mip levels.");
			}
		}
	}

	texture_desc.ArraySize = static_cast<UINT>(path_arr.size());
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(
		&texture_desc,
		nullptr,
		this->texture_arr.GetAddressOf()
	);
	CHECK(hr);

	// texture array 에 textures 의 내용 복사
	for (int i = 0; i < textures.size(); i++) {
		context->CopySubresourceRegion(
			this->texture_arr.Get(),
			D3D11CalcSubresource(
				0,
				static_cast<UINT>(i),
				texture_desc.MipLevels
			),
			0,
			0,
			0,
			textures[i],
			0,
			nullptr
		);
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(view_desc));
	view_desc.Format = texture_desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	view_desc.Texture2DArray.MostDetailedMip = 0;
	view_desc.Texture2DArray.MipLevels = texture_desc.MipLevels;
	view_desc.Texture2DArray.FirstArraySlice = 0;
	view_desc.Texture2DArray.ArraySize = texture_desc.ArraySize;

	hr = device->CreateShaderResourceView(
		this->texture_arr.Get(),
		&view_desc,
		this->shader_resource_view.GetAddressOf()
	);
	CHECK(hr);

	// 리소스 해제
	for (ID3D11Texture2D* tex : textures) {
		if (tex) {
			tex->Release();
		}
	}
}

ComPtr<ID3D11ShaderResourceView> TextureArray::getComPtr()
{
	return this->shader_resource_view;
}
