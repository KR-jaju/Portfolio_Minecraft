#pragma once
class DepthMap
{
public:
	DepthMap(ComPtr<ID3D11Device>& device);

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Texture2D> depth_texture;
	ComPtr<ID3D11ShaderResourceView> depth_SRV;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view;
};

