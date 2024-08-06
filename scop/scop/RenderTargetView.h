#pragma once

class RenderTargetView {
public:
	RenderTargetView(ComPtr<ID3D11Device> const& device, ID3D11Texture2D* texture);
private:
	ComPtr<ID3D11Device>			device;
	ComPtr<ID3D11RenderTargetView>	view;
};
