#include "pch.h"
#include "SwapChain.h"
#include "Context.h"

SwapChain::SwapChain(Context const& context, uint32 width, uint32 height)
{
	HWND const	handle = context.getWindowHandle();
	ID3D11Device* const device = context.getDevice();
	ComPtr<IDXGIFactory> factory;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = static_cast<UINT>(width);
	desc.BufferDesc.Height = static_cast<UINT>(height);
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = handle;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	CHECK(hr);
	hr = factory->CreateSwapChain(device, &desc, this->swap_chain.GetAddressOf());
	CHECK(hr);
}