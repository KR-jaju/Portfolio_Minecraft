#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd, UINT width, UINT height)
	: hwnd(hwnd), width(width), height(height)
{
	createDeviceAndSwapChain();
	createRenderTargetView();
	setViewPort();
}

Graphics::~Graphics()
{
}

void Graphics::renderBegin()
{
	this->context->OMSetRenderTargets(
		1,
		this->render_target_view.GetAddressOf(),
		nullptr
	);
	this->context->ClearRenderTargetView(
		this->render_target_view.Get(),
		this->clear_color
	);
	this->context->RSSetViewports(1, &(this->view_port));
}

void Graphics::renderEnd()
{
	this->swap_chain->Present(1, 0);
}

void Graphics::setClearColor(float r, float g, float b, float a)
{
	this->clear_color[0] = r;
	this->clear_color[1] = g;
	this->clear_color[2] = b;
	this->clear_color[3] = a;
}

ComPtr<ID3D11Device> Graphics::getDevice() const
{
	return this->device;
}

ComPtr<ID3D11DeviceContext> Graphics::getContext() const
{
	return this->context;
}

void Graphics::createDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferDesc.Width = this->width;
	desc.BufferDesc.Height = this->height;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = this->hwnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		this->swap_chain.GetAddressOf(),
		this->device.GetAddressOf(),
		nullptr,
		this->context.GetAddressOf()
	);
	CHECK(hr);
}

void Graphics::createRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = this->swap_chain->GetBuffer(
		0, 
		__uuidof(ID3D11Texture2D), 
		reinterpret_cast<void **>(backBuffer.GetAddressOf())
	);
	CHECK(hr);

	hr = this->device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		this->render_target_view.GetAddressOf()
	);
	CHECK(hr);
}

void Graphics::setViewPort()
{
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = static_cast<float>(this->width);
	this->view_port.Height = static_cast<float>(this->height);
	this->view_port.MinDepth = 0.0f;
	this->view_port.MaxDepth = 1.0f;
}
