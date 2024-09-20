#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd, UINT width, UINT height)
	: hwnd(hwnd), width(width), height(height)
{
	createDeviceAndSwapChain();
	createRenderTargetView();
	createDepthStencilView();
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
		this->depth_stencil_view.Get()
	);
	/*this->context->OMSetRenderTargets(0, NULL, depth stencil view);
	뎁스맵만 만드는 경우 아래처럼 초기화는 필요하고 위처럼 랜더타겟을 넣어 줄 필요
	없다.*/
	this->context->ClearRenderTargetView(
		this->render_target_view.Get(),
		this->clear_color
	);
	this->context->ClearDepthStencilView(
		this->depth_stencil_view.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		0
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

ComPtr<ID3D11RenderTargetView> Graphics::getRenderTargetVew() const
{
	return this->render_target_view;
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
	desc.BufferCount = 2;
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

void Graphics::createDepthStencilView()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = this->width;
	desc.Height = this->height;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	this->device->CreateTexture2D(
		&desc,
		0,
		this->texture_2d.GetAddressOf()
	);

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	ZeroMemory(
		&depth_stencil_view_desc, 
		sizeof(depth_stencil_view_desc)
	);
	depth_stencil_view_desc.Format = desc.Format;
	depth_stencil_view_desc.ViewDimension =
		D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	this->device->CreateDepthStencilView(
		this->texture_2d.Get(),
		&depth_stencil_view_desc,
		this->depth_stencil_view.GetAddressOf()
	);
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
