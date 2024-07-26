#pragma once
class SamplerState
{
public:
	SamplerState(ComPtr<ID3D11Device> device);
	~SamplerState();
	ComPtr<ID3D11SamplerState> getComPtr() const;
	void create(const D3D11_SAMPLER_DESC& desc);
	void create();
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11SamplerState> sampler_state;
};

