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
	const D3D11_FILL_MODE& fillMode, 
	const D3D11_CULL_MODE& cullMode
)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = fillMode;
	desc.CullMode = cullMode;
	desc.FrontCounterClockwise = false;
	HRESULT hr = this->device->CreateRasterizerState(
		&desc,
		this->rasterizer_state.GetAddressOf()
	);
	CHECK(hr);
}


