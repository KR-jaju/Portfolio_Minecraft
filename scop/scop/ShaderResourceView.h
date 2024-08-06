#pragma once


class ShaderResourceView {
public:
	ShaderResourceView(ComPtr<ID3D11Device> const& device, ID3D11Texture1D* texture);
	ShaderResourceView(ComPtr<ID3D11Device> const& device, ID3D11Texture2D* texture);
	ShaderResourceView(ComPtr<ID3D11Device> const& device, ID3D11Texture3D* texture);
private:
	ComPtr<ID3D11Device>				device;
	ComPtr<ID3D11ShaderResourceView>	view;
};
