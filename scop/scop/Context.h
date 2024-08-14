#pragma once

class Context {
public:
	Context(HWND handle);
	Context(Context const&) = delete;
	
	ID3D11Device* getDevice() const;
	ID3D11DeviceContext* getDeviceContext() const;
	HWND	getWindowHandle() const;
private:
	HWND	handle;
	ComPtr<ID3D11Device>	device;
	ComPtr<ID3D11DeviceContext>	device_context;
};
