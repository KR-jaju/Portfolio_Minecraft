#include "pch.h"
#include "SwapChain.h"
#include "Context.h"

namespace d3d11
{
	static ComPtr<IDXGIFactory> createFactory()
	{
		void* ptr = nullptr;
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), &ptr);

		CHECK(hr);
		return (static_cast<IDXGIFactory*>(ptr));
	}
	static ComPtr<IDXGISwapChain> createSwapChain(ID3D11Device* device, HWND handle, IDXGIFactory *factory, uint32 width, uint32 height)
	{
		IDXGISwapChain* result = nullptr;
		DXGI_SWAP_CHAIN_DESC desc = {};

		desc.BufferDesc.Width = static_cast<UINT>(width);
		desc.BufferDesc.Height = static_cast<UINT>(height);
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
		desc.OutputWindow = handle;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		HRESULT hr = factory->CreateSwapChain(device, &desc, &result);
		CHECK(hr);
		return (result);
	}
	static ComPtr<ID3D11Texture2D> getBackBuffer(IDXGISwapChain* swap_chain)
	{
		ComPtr<ID3D11Texture2D> texture;
		HRESULT hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(texture.GetAddressOf()));
		CHECK(hr);
		return (texture);
	}
	static ComPtr<ID3D11RenderTargetView> createRenderTargetView(ID3D11Device* device, ID3D11Texture2D* texture)
	{
		ComPtr<ID3D11RenderTargetView> result;
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};

		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 추후에 multisample이 추가될 수 있음.
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Texture2D.MipSlice = 0;
		HRESULT hr = device->CreateRenderTargetView(texture, &desc, result.GetAddressOf());
		CHECK(hr);
		return (result);
	}
	static ComPtr<ID3D11Texture2D> createDepthTexture(ID3D11Device* device, uint32 width, uint32 height)
	{
		ComPtr<ID3D11Texture2D> result;
		D3D11_TEXTURE2D_DESC desc = {};

		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // shader resouce는 못하나봄
		desc.CPUAccessFlags = 0;
		desc.Width = static_cast<UINT>(width);
		desc.Height = static_cast<UINT>(height);
		desc.MipLevels = 1; // max level 하나만 있음
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0; // non-multisample의 기본 설정임
		desc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, result.GetAddressOf());
		CHECK(hr);
		return (result);
	}
	static ComPtr<ID3D11DepthStencilView> createDepthStencilView(ID3D11Device* device, ID3D11Texture2D* texture)
	{
		ComPtr<ID3D11DepthStencilView> result;
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};

		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = 0;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = device->CreateDepthStencilView(texture, &desc, result.GetAddressOf());
		CHECK(hr);
		return (result);
	}

	SwapChain::SwapChain(Device const& device, HWND handle, uint32 width, uint32 height)
	{
		ComPtr<IDXGIFactory> const factory = createFactory();
		ComPtr<IDXGISwapChain> const swap_chain = createSwapChain(device.getDevice(), handle, factory.Get(), width, height);
		ComPtr<ID3D11Texture2D> const back_buffer = getBackBuffer(swap_chain.Get());
		ComPtr<ID3D11RenderTargetView> const rtv = createRenderTargetView(device.getDevice(), back_buffer.Get());
		ComPtr<ID3D11Texture2D> const depth_texture = createDepthTexture(device.getDevice(), width, height);
		ComPtr<ID3D11DepthStencilView> const dsv = createDepthStencilView(device.getDevice(), depth_texture.Get());

		this->swap_chain = std::move(swap_chain);
		this->rtv = std::move(rtv);
		this->depth_texture = std::move(depth_texture);
		this->dsv = std::move(dsv);
	}

	void	SwapChain::present()
	{
		this->swap_chain->Present(1, 0);
	}

	IDXGISwapChain* SwapChain::getInternalResource() const
	{
		return (this->swap_chain.Get());
	}

	ID3D11RenderTargetView* SwapChain::getRenderTargetView(uint32 idx) const
	{
		return (this->rtv.Get());
	}

	ID3D11DepthStencilView* SwapChain::getDepthStencilView() const
	{
		return (this->dsv.Get());
	}
}
