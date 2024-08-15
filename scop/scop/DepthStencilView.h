#pragma once

#include "Context.h"
#include "DepthStencilTexture.h"

class DepthStencilView {
public:
	DepthStencilView(Context const& context, DepthStencilTexture const& texture);
	ID3D11DepthStencilView* getInternalResource() const;
private:
	ID3D11Device*			device;
	ComPtr<ID3D11DepthStencilView>	view;
};
