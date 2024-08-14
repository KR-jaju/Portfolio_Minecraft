#pragma once

#include "RenderTarget.h"

class Context;

class SwapChain : public RenderTarget {
public:
	SwapChain(Context const& context, uint32 width, uint32 height);
	ID3D11Texture2D* getInternalResource() const;
private:
	ComPtr<IDXGISwapChain>	swap_chain;
};
