#pragma once
class DepthMap
{
public:
	DepthMap(ComPtr<ID3D11Device> const& device, UINT width, UINT height);
	ComPtr<ID3D11DepthStencilView> getDepthStencilView();
	ComPtr<ID3D11ShaderResourceView> getShaderResourceView();
	D3D11_VIEWPORT getViewPort();
private:// test
	void setViewPort();
private:
	UINT width;
	UINT height;
	D3D11_VIEWPORT view_port = { 0 };
	ComPtr<ID3D11Texture2D> depth_texture;
	ComPtr<ID3D11ShaderResourceView> depth_SRV;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view;
};

