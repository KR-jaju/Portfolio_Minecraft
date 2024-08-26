#pragma once

#include "RenderTarget.h"

namespace d3d11
{
	class Context;
	class Device
	{
	public:
		Device();
		::ID3D11Device* getDevice() const;
		::ID3D11DeviceContext* getDeviceContext() const;
		void	execute(Context const& context);
	private:
		ComPtr<::ID3D11Device>	device;
		ComPtr<::ID3D11DeviceContext>	device_context;
	};
}