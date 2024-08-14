#pragma once

class Context;

class SwapChain {
public:
	SwapChain(Context const& context, uint32 width, uint32 height);
private:
	ComPtr<IDXGISwapChain>	swap_chain;
};
