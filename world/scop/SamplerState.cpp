#include "pch.h"
#include "SamplerState.h"

SamplerState::SamplerState(ComPtr<ID3D11Device> device)
	: device(device)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;



	//desc.MipLODBias = 1.0f;                         // Mipmap 레벨 오프셋
	//desc.MaxAnisotropy = 0;                         // 이방성 필터링 비율 (1일 때 비활성화)
	//desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // 깊이 비교 없음
	//desc.MinLOD = 0;                                // 최소 LOD (밉맵 레벨)
	//desc.MaxLOD = D3D11_FLOAT32_MAX;                // 최대 LOD (모든 밉맵 사용)








	HRESULT hr = this->device->CreateSamplerState(
		&desc,
		this->sampler_state.GetAddressOf()
	);
	CHECK(hr);
}

SamplerState::SamplerState(
	ComPtr<ID3D11Device> device,
	D3D11_SAMPLER_DESC const& desc
)
	: device(device)
{
	HRESULT hr = this->device->CreateSamplerState(
		&desc,
		this->sampler_state.GetAddressOf()
	);
	CHECK(hr);
}

SamplerState::~SamplerState()
{
}

ComPtr<ID3D11SamplerState> SamplerState::getComPtr() const
{
	return this->sampler_state;
}
