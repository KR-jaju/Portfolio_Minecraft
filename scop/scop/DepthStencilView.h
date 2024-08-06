#pragma once

class DepthStencilView {
public:
	DepthStencilView(ComPtr<ID3D11Device> device, ID3D11Texture2D* texture);
private:
	ComPtr<ID3D11Device>			device;
	ComPtr<ID3D11DepthStencilView>	view;
};
