#pragma once

#include "Context.h"
#include "RenderTargetTexture.h"

class RenderTargetView {
public:
	RenderTargetView(Context const& context, RenderTargetTexture const& texture);
private:
	ComPtr<ID3D11Device>			device;
	ComPtr<ID3D11RenderTargetView>	view;
};
