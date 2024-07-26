#pragma once

#include "Texture.h"

class TextureManager
{
public:
	TextureManager(ComPtr<ID3D11Device> device);
	~TextureManager();
	bool addTexture(
		const string& name,
		const wstring& path
	);
	bool removeTexture(const string& name);
	shared_ptr<Texture> getTexture(const string& name);

private:
	bool checkTextureBook(const string& name) const;

private:
	ComPtr<ID3D11Device> device;
	map<string, shared_ptr<Texture>> texture_book;
};

