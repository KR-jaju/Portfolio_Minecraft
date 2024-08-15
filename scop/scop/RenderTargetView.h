#pragma once

#include "Context.h"
#include "RenderTargetTexture.h"

class SwapChain;

class RenderTargetView {
public:
	RenderTargetView(Context const& context, RenderTargetTexture const& texture);
	RenderTargetView(Context const& context, SwapChain const& swap_chain);
	ID3D11RenderTargetView* getInternalResource() const;
	ID3D11RenderTargetView* const* getAddressOf() const;
private:
	ComPtr<ID3D11Device>			device;
	ComPtr<ID3D11RenderTargetView>	view;
};
