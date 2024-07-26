#include "pch.h"
#include "TextureManager.h"

TextureManager::TextureManager(ComPtr<ID3D11Device> device)
	: device(device)
{
}

TextureManager::~TextureManager()
{
}

bool TextureManager::addTexture(
	const string& name, 
	const wstring& path
)
{
	if (this->checkTextureBook(name))
		return false;
	shared_ptr<Texture> texture = make_shared<Texture>(this->device);
	texture->create(path);
	this->texture_book[name] = texture;
	return true;
}

bool TextureManager::removeTexture(const string& name)
{
	if (this->checkTextureBook(name) == false)
		return false;
	this->texture_book.erase(name);
	return true;
}

shared_ptr<Texture> TextureManager::getTexture(const string& name)
{
	if (this->checkTextureBook(name) == false)
		return nullptr;
	return this->texture_book[name];
}

bool TextureManager::checkTextureBook(const string& name) const
{
	map<string, shared_ptr<Texture>>::const_iterator it;
	it = this->texture_book.find(name);
	if (it == this->texture_book.end())
		return false;
	return true;
}
