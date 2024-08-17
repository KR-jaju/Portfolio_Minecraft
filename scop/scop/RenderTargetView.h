#pragma once

#include "Context.h"
#include "RenderTargetTexture.h"

class SwapChain;

class RenderTargetView {
public:
	RenderTargetView(Context const& context, RenderTargetTexture const& texture);
	RenderTargetView(Context const& context, SwapChain const& swap_chain);
	void clear(float r, float g, float b, float a);
	ID3D11RenderTargetView* getInternalResource() const;
	ID3D11RenderTargetView* const* getAddressOf() const;
private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	ComPtr<ID3D11RenderTargetView>	view;
};
