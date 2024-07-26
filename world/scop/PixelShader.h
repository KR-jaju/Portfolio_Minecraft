#pragma once
#include "Shader.h"
class PixelShader : public Shader
{
public:
	PixelShader(ComPtr<ID3D11Device> device);
	virtual ~PixelShader();
	ComPtr<ID3D11PixelShader> getComPtr() const;
	virtual void create(
		const wstring& path,
		const string& name,
		const string& version
	) override;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11PixelShader> pixel_shader;
};

