#include "pch.h"
#include "DeferredBuffer.h"

DeferredBuffer::DeferredBuffer(int rtv_num)
{
	this->rtv_num = rtv_num;
	this->RTVs.resize(rtv_num);
	this->SRVs.resize(rtv_num);
}

DeferredBuffer::~DeferredBuffer()
{
}

void DeferredBuffer::setRTVsAndSRVs(
	ComPtr<ID3D11Device> device,
	int width,
	int height
)
{
	D3D11_TEXTURE2D_DESC t_desc;
	ZeroMemory(&t_desc, sizeof(t_desc));
	t_desc.Width = width;
	t_desc.Height = height;
	t_desc.MipLevels = 1;
	t_desc.ArraySize = 1;
	t_desc.Usage = D3D11_USAGE_DEFAULT;
	t_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	t_desc.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	t_desc.CPUAccessFlags = 0;
	t_desc.SampleDesc.Count = 1;
	t_desc.SampleDesc.Quality = 0;
	HRESULT hr;
	ComPtr<ID3D11Texture2D> tex[8];
	for (int i = 0; i < this->rtv_num; i++) {
		hr = device->CreateTexture2D(&t_desc,
			nullptr, tex[i].GetAddressOf());
	}
	CHECK(hr);

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	ZeroMemory(&rtv_desc, sizeof(rtv_desc));
	rtv_desc.Format = t_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	for (int i = 0; i < this->rtv_num; i++) {
		hr = device->CreateRenderTargetView(tex[i].Get(),
			&rtv_desc, this->RTVs[i].GetAddressOf());
		CHECK(hr);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(srv_desc));
	srv_desc.Format = t_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = 1;
	for (int i = 0; i < rtv_num; i++) {
		hr = device->CreateShaderResourceView(
			tex[i].Get(), &srv_desc, this->SRVs[i].GetAddressOf());
		CHECK(hr);
	}
}

ComPtr<ID3D11RenderTargetView> DeferredBuffer::getRTV(int idx)
{
	return this->RTVs[idx];
}


ComPtr<ID3D11ShaderResourceView> DeferredBuffer::getSRV(int idx)
{
	return this->SRVs[idx];
}

int DeferredBuffer::getCnt()
{
	return this->rtv_num;
}
