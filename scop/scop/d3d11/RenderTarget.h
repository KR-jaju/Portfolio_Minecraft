#pragma once

namespace d3d11
{
	class Context;
	class RenderTarget
	{
	public:
		virtual ~RenderTarget() = default;
		//d3d11 private
		virtual ID3D11RenderTargetView* getRenderTargetView(uint32 idx) const = 0;
		virtual ID3D11DepthStencilView* getDepthStencilView() const = 0;
	private:
	};
}