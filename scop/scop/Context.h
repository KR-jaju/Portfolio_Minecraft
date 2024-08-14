#pragma once

class Context {
public:
	Context();
	Context(Context const&) = delete;
	
	ID3D11Device* getDevice() const;
	ID3D11DeviceContext* getDeviceContext() const;
private:
	ComPtr<ID3D11Device>	device;
	ComPtr<ID3D11DeviceContext>	device_context;
};