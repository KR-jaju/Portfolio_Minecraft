#pragma once

#include "RenderTarget.h"

class Context;

class SwapChain {
public:
	SwapChain(Context const& context, uint32 width, uint32 height);
	ID3D11Texture2D* getBackBuffer() const;
	IDXGISwapChain* getInternalResource() const;
private:
	ComPtr<IDXGISwapChain>	swap_chain;
};
