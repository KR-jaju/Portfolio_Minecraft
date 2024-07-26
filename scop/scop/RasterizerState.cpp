#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: device(device)
{
}

RasterizerState::~RasterizerState()
{
}

ComPtr<ID3D11RasterizerState> RasterizerState::getComPtr() const
{
	return this->rasterizer_state;
}

void RasterizerState::create(
	const D3D11_FILL_MODE& fill_mode, 
	const D3D11_CULL_MODE& cull_mode
)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = fill_mode;
	desc.CullMode = cull_mode;
	desc.FrontCounterClockwise = false;
	HRESULT hr = this->device->CreateRasterizerState(
		&desc,
		this->rasterizer_state.GetAddressOf()
	);
	CHECK(hr);
}


