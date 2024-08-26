#include "pch.h"
#include "Context.h"
#include "Device.h"

#include "SwapChain.h"
#include "RenderTarget.h";

namespace d3d11
{
	Context::Context(Device const& device)
		: device_context()
	{
		device.getDevice()->CreateDeferredContext(0, this->device_context.GetAddressOf());
	}

	void	Context::setViewport(uint32 x, uint32 y, uint32 w, uint32 h)
	{
		::ID3D11DeviceContext* const context = this->device_context.Get();
		D3D11_VIEWPORT const	viewport = {
			x, y, w, h, 0.0f, 1.0f
		};
		context->RSSetViewports(1, &viewport);
	}

	void	Context::setRenderTarget(RenderTarget const* target)
	{
		this->render_target = target;
	}

	void	Context::clearColor(uint32 idx, float r, float g, float b, float a)
	{
		ID3D11DeviceContext* internal_context = this->device_context.Get();
		ID3D11RenderTargetView* rtv = this->render_target->getRenderTargetView(idx);
		float rgba[] = { r, g, b, a };

		internal_context->ClearRenderTargetView(rtv, rgba);
	}
	void	Context::clearDepthStencil(float depth, uint32 stencil)
	{
		ID3D11DeviceContext* internal_context = this->device_context.Get();
		ID3D11DepthStencilView* dsv = this->render_target->getDepthStencilView();

		internal_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	}
	::ID3D11DeviceContext* Context::getInternalResource() const
	{
		return (this->device_context.Get());
	}
	void	Context::setPrimitive(Primitive const* primitive)
	{
		this->primitive = primitive;
	}
}