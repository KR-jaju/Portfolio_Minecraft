#include "pch.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"

DeferredGraphics::DeferredGraphics(
	HWND hwnd, 
	UINT width, 
	UINT height
) : hWnd(hwnd), width(width), height(height)
{
	this->createSwapChainAndDevice();
	this->createRTV();
	this->createDepthStencilView();
	this->setViewPort();
}

DeferredGraphics::~DeferredGraphics()
{
}

void DeferredGraphics::setViewPort(D3D11_VIEWPORT const& view_port)
{
	this->context->RSSetViewports(1, &(view_port));
}

void DeferredGraphics::renderBegin(
	DeferredBuffer* d_buffer,
	ComPtr<ID3D11DepthStencilView> dsv
)
{
	int cnt = 1;
	if (d_buffer)
		cnt = d_buffer->getCnt();
	vector<ID3D11RenderTargetView*> views(cnt);
	if (d_buffer) {
		for (int i = 0; i < cnt; i++)
			views[i] = d_buffer->getRTV(i).Get();
		for (int i = 0; i < cnt; i++) {
			this->context->ClearRenderTargetView(
				views[i],
				this->clear_color
			);
		}
	}
	else {
		views[0] = this->defer_rtv.Get();
		this->context->ClearRenderTargetView(
			views[0],
			this->clear_color
		);
	}
	ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	if (dsv)
		depth_stencil_view = dsv;
	else
		depth_stencil_view = this->DSV;
	this->context->OMSetRenderTargets(
		cnt,
		views.data(),
		depth_stencil_view.Get()
	);
	
	this->context->ClearDepthStencilView(
		depth_stencil_view.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		0
	);
	this->context->RSSetViewports(1, &(this->view_port));
}

void DeferredGraphics::renderEnd()
{
	this->swap_chain->Present(1, 0);
}

void DeferredGraphics::createSwapChainAndDevice()
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
	desc.BufferUsage = DXGI_USAGE_SHADER_INPUT |
		DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.OutputWindow = this->hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HREFTYPE hr = D3D11CreateDeviceAndSwapChain(
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

void DeferredGraphics::createRTV()
{
	HRESULT hr;
	ComPtr<ID3D11Texture2D> tex;
	for (int i = 0; i < 3; i++) {
		hr = this->swap_chain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(tex.GetAddressOf())
		);
		CHECK(hr);
	}
	hr = this->device->CreateRenderTargetView(
		tex.Get(),
		nullptr,
		this->defer_rtv.GetAddressOf()
	);
	CHECK(hr);
}

void DeferredGraphics::createDepthStencilView()
{
	ComPtr<ID3D11Texture2D> depth_texture;
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
	
	HRESULT hr = this->device->CreateTexture2D(
		&desc, nullptr, depth_texture.GetAddressOf());
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));
	dsv_desc.Format = desc.Format;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	hr = this->device->CreateDepthStencilView(
		depth_texture.Get(), &dsv_desc, this->DSV.GetAddressOf());
	CHECK(hr);
}

void DeferredGraphics::setViewPort()
{
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = static_cast<float>(this->width);
	this->view_port.Height = static_cast<float>(this->height);
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
}

ComPtr<ID3D11Device> DeferredGraphics::getDevice()
{
	return this->device;
}

ComPtr<ID3D11DeviceContext> DeferredGraphics::getContext()
{
	return this->context;
}

ComPtr<ID3D11DepthStencilView> DeferredGraphics::getDSV()
{
	return this->DSV;
}
