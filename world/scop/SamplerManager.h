#pragma once

#include "SamplerState.h"

class SamplerState;

class SamplerManager
{
public:
	SamplerManager(ComPtr<ID3D11Device> device);
	~SamplerManager();
	bool addSamplerState(
		const string& name,
		const D3D11_SAMPLER_DESC& desc
	);
	bool addSamplerState(const string& name);
	bool removeSamplerState(const string& name);
	shared_ptr<SamplerState> getSamplerState(const string& name);

private:
	bool checkSamplerStateBook(const string& name) const;

private:
	ComPtr<ID3D11Device> device;
	map<string, shared_ptr<SamplerState>> sampler_state_book;
};

