#pragma once
#include "ResourceBase.h"

class TextureResource : public ResourceBase
{
	using Super = ResourceBase;
public:
	TextureResource(ComPtr<ID3D11Device> device);
	~TextureResource();

	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return this->shader_resource_view; }
	vec2 GetTextureSize() { return this->size; }
	
	void Create(const wstring& path);

	vec2 GetSize() { return this->size; }

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	vec2 size = { 0.f, 0.f };

};


