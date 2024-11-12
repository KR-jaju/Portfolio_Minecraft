#include "pch.h"
#include "EntityDataAsset.h"
#include "DeferredGraphics.h"

EntityDataAsset::EntityDataAsset(shared_ptr<DeferredGraphics> graphics)
	: device(graphics->getDevice())
{
	//ComPtr<ID3D11Device> device = graphics->getDevice();

	//graphics->getContext();
}


void	EntityDataAsset::registerGeometry(std::string const& key, Model const& model)
{
	if (this->table.count(key))
		throw std::runtime_error("Duplicate key in EntityDataAsset");

	this->table.emplace(key, make_shared<EntityGeometry>(this->device, model.vertices, model.indices));
}


shared_ptr<EntityGeometry>	EntityDataAsset::getGeometry(std::string const& key)
{
	if (!this->table.count(key))
		throw std::runtime_error("Target geometry does not exist");
	return this->table.at(key);
}

void	EntityDataAsset::registerTexture(std::string const& key, std::wstring const& path)
{
	if (this->texture_table.count(key))
		throw std::runtime_error("Duplicate key in EntityDataAsset");

	this->texture_table.emplace(key, make_shared<Texture>(this->device, path));
}


shared_ptr<Texture>	EntityDataAsset::getTexture(std::string const& key)
{
	if (!this->texture_table.count(key))
		throw std::runtime_error("Target geometry does not exist");
	return this->texture_table.at(key);
}
