#include "pch.h"
#include "Device.h"
#include "Context.h"

namespace d3d11
{
	Device::Device()
		: device(), device_context()
	{
		DXGI_SWAP_CHAIN_DESC desc = {};
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			this->device.GetAddressOf(),
			nullptr,
			this->device_context.GetAddressOf()
		);
		CHECK(hr);
	}

	::ID3D11Device* Device::getDevice() const
	{
		return (this->device.Get());
	}

	::ID3D11DeviceContext * Device::getDeviceContext() const
	{
		return (this->device_context.Get());
	}

	void	Device::execute(Context const& context)
	{
		::ID3D11DeviceContext* immediate_context = this->device_context.Get();
		::ID3D11DeviceContext* deferred_context = context.getInternalResource();
		ComPtr<::ID3D11CommandList> command_list = nullptr;

		deferred_context->FinishCommandList(FALSE, &command_list);
		immediate_context->ExecuteCommandList(command_list.Get(), TRUE);
		// deferred_context.ClearState() // 필요에 따라 지연 컨텍스트 상태 초기화
	}
}