#pragma once
class RasterizerState
{
public:
	RasterizerState(
		ComPtr<ID3D11Device> device,
		D3D11_FILL_MODE fill_mode,
		D3D11_CULL_MODE cull_mode
	);
	RasterizerState(RasterizerState const&) = delete;
	~RasterizerState();
	RasterizerState& operator=(RasterizerState const&) = delete;
	ComPtr<ID3D11RasterizerState> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11RasterizerState> rasterizer_state;
};

