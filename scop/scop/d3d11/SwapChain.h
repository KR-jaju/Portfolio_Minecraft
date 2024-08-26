#pragma once

#include "RenderTarget.h"
#include "Device.h"

namespace d3d11
{
	class SwapChain : public RenderTarget {
	public:
		SwapChain(Device const& device, HWND handle, uint32 width, uint32 height);
		void	present();

		//d3d private
		IDXGISwapChain* getInternalResource() const;
		ID3D11RenderTargetView* getRenderTargetView(uint32 idx) const;
		ID3D11DepthStencilView* getDepthStencilView() const;
	private:
		ComPtr<IDXGISwapChain>	swap_chain;
		ComPtr<ID3D11RenderTargetView> rtv;
		ComPtr<ID3D11Texture2D> depth_texture;
		ComPtr<ID3D11DepthStencilView> dsv;
	};
}

