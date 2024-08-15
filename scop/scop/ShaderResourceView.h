#pragma once

#include "Context.h"
#include "ShaderResourceTexture.h"

class ShaderResourceView {
public:
	ShaderResourceView(Context const& context, ShaderResourceTexture const& texture);
	ID3D11ShaderResourceView* getInternalResource() const;
private:
	ID3D11Device*				device;
	ComPtr<ID3D11ShaderResourceView>	view;
};
