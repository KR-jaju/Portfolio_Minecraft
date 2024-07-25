#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(ComPtr<ID3D11Device> device)
	: device(device)
{
}

BlendState::~BlendState()
{
}

void BlendState::create(
	D3D11_RENDER_TARGET_BLEND_DESC blend_desc, 
	float factor
)
{
	this->blend_factor = factor;

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0] = blend_desc;

	HRESULT hr = this->device->CreateBlendState(
		&desc,
		this->blend_state.GetAddressOf()
	);
}

ComPtr<ID3D11BlendState> BlendState::getComPtr() const
{
	return this->blend_state;
}
