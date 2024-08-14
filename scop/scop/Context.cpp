#include "pch.h"
#include "Context.h"

Context::Context(HWND handle)
	: handle(handle), device(), device_context()
{
	HRESULT hr = D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr, 
		0, 
		nullptr, 
		0, 
		D3D11_SDK_VERSION, 
		this->device.GetAddressOf(), 
		nullptr, 
		this->device_context.GetAddressOf());
	CHECK(hr);
}


ID3D11Device* Context::getDevice() const 
{
	return (this->device.Get());
}

ID3D11DeviceContext* Context::getDeviceContext() const
{
	return (this->device_context.Get());
}

HWND	Context::getWindowHandle() const
{
	return (this->handle);
}

void	Context::setViewport(uint32 x, uint32 y, uint32 w, uint32 h)
{
	D3D11_VIEWPORT const	viewport = {
		x, y, w, h, 0.0f, 1.0f
	};
	this->device_context->RSSetViewports(1, &viewport);
}
