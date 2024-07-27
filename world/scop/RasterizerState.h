#pragma once
class RasterizerState
{
public:
	RasterizerState(
		ComPtr<ID3D11Device> device,
		D3D11_FILL_MODE const& fill_mode,
		D3D11_CULL_MODE const& cull_mode
	);
	~RasterizerState();
	ComPtr<ID3D11RasterizerState> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11RasterizerState> rasterizer_state;
};

