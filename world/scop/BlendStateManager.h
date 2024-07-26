#pragma once

#include "BlendState.h"

class BlendStateManager
{
public:
	BlendStateManager(ComPtr<ID3D11Device> device);
	~BlendStateManager();
	bool addBlendState(const string& name);
	bool addBlendState(
		const string& name,
		const D3D11_RENDER_TARGET_BLEND_DESC& blend_desc,
		const float& factor
	);
	bool removeBlendState(const string& name);
	shared_ptr<BlendState> getBlendState(const string& name);

private:
	bool checkBlendStateBook(const string& name) const;

private:
	ComPtr<ID3D11Device> device;
	map<string, shared_ptr<BlendState>> blend_state_book;
};

