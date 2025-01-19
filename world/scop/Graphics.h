#pragma once
class Graphics
{
public:
	Graphics(HWND hwnd, UINT width, UINT height);
	~Graphics();

	void renderBegin();
	void present();
	void setClearColor(float r, float g, float b, float a);
	ComPtr<ID3D11Device> getDevice() const;
	ComPtr<ID3D11DeviceContext> getContext() const;
	ComPtr<ID3D11Texture2D> getSwapChainTexture() const;

private:
	void createDeviceAndSwapChain();
	void setViewPort();

private:
	HWND hwnd;
	UINT width;
	UINT height;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swap_chain;

	D3D11_VIEWPORT view_port = { 0 };
	float clear_color[4] = { 0.f, 0.f, 0.f, 1.f };
};

