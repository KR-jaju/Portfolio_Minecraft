#pragma once
#include "Shader.h"
class VertexShader : public Shader
{
public:
	VertexShader(ComPtr<ID3D11Device> device);
	virtual ~VertexShader();
	ComPtr<ID3D11VertexShader> getComPtr() const;
	virtual void create(
		const wstring& path,
		const string& name,
		const string& version
	) override;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11VertexShader> vertex_shader;
};

