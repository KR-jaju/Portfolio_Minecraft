#include "pch.h"
#include "SamplerState.h"

SamplerState::SamplerState(ComPtr<ID3D11Device> device)
	: device(device)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
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
