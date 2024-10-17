#include "pch.h"
#include "Ssao.h"
#include "MathHelper.h"
#include "DirectXPackedVector.h"

Ssao::Ssao(ComPtr<ID3D11Device> device)
{
	this->buildOffset();
	this->buildRandomVecTexture(device);
}

void Ssao::buildOffset()
{
	mOffsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	mOffsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	mOffsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	mOffsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	mOffsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	mOffsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	mOffsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	mOffsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = MathHelper::RandF(0.25f, 1.0f);

		XMVECTOR v = s * XMVector4Normalize(XMLoadFloat4(&mOffsets[i]));

		XMStoreFloat4(&mOffsets[i], v);
	}
}

void Ssao::buildRandomVecTexture(ComPtr<ID3D11Device> device)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = 256;
	desc.Height = 256;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = 256 * sizeof(DirectX::PackedVector::XMCOLOR);

	DirectX::PackedVector::XMCOLOR color[256 * 256];
	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			XMFLOAT3 v(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());

			color[i * 256 + j] = DirectX::PackedVector::XMCOLOR(v.x, v.y, v.z, 0.0f);
		}
	}

	initData.pSysMem = color;

	ComPtr<ID3D11Texture2D> tex = 0;
	HRESULT hr;
	hr = device->CreateTexture2D(&desc, &initData, tex.GetAddressOf());
	CHECK(hr);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(view_desc));
	view_desc.Format = desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(tex.Get(), &view_desc, 
		this->random_vec_SRV.GetAddressOf());
	CHECK(hr);
}
