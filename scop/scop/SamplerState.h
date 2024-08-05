#pragma once
class SamplerState
{
public:
	SamplerState(ComPtr<ID3D11Device> device);
	SamplerState(
		ComPtr<ID3D11Device> device,
		D3D11_SAMPLER_DESC const& desc
	);
	SamplerState(SamplerState const&) = delete;
	~SamplerState();
	SamplerState& operator=(SamplerState const&) = delete;
	ComPtr<ID3D11SamplerState> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11SamplerState> sampler_state;
};

