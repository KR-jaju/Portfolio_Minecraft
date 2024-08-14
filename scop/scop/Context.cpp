#include "pch.h"
#include "Context.h"

Context::Context() {

}


ID3D11Device* Context::getDevice() const 
{
	return (this->device.Get());
}

ID3D11DeviceContext* Context::getDeviceContext() const
{
	return (this->device_context.Get());
}