#pragma once

#include "Context.h"
#include "DepthStencilTexture.h"

class DepthStencilView {
public:
	DepthStencilView(Context const& context, DepthStencilTexture const& texture);
	void	clear(float depth, float stencil);
	void	clearDepth(float depth);
	void	clearStencil(float stencil);
	ID3D11DepthStencilView* getInternalResource() const;
private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	ComPtr<ID3D11DepthStencilView>	view;
};
