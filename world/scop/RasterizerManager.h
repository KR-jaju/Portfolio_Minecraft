#pragma once

#include "RasterizerState.h"

class RasterizerManager
{
public:
	RasterizerManager(ComPtr<ID3D11Device> device);
	~RasterizerManager();
	bool addRasterizerState(
		const string& name,
		const D3D11_FILL_MODE& fill_mode,
		const D3D11_CULL_MODE& cull_mode
	);
	bool removeRasterizerState(const string& name);
	shared_ptr<RasterizerState> getRasterizerState(const string& name);

private:
	bool checkRasterizerStateBook(const string& name) const;

private:
	ComPtr<ID3D11Device> device;
	map<string, shared_ptr<RasterizerState>> rasterizer_state_book;
};

