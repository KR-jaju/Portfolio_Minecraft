#pragma once
class RasterizerState
{
public:
	RasterizerState(ComPtr<ID3D11Device> device);
	~RasterizerState();
	ComPtr<ID3D11RasterizerState> getComPtr() const;
	void create(
		const D3D11_FILL_MODE& fillMode,
		const D3D11_CULL_MODE& cullMode
	);
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11RasterizerState> rasterizer_state;
};

