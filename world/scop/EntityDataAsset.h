#pragma once

#include "Buffer.h"
#include "EntityVertex.h"
#include "SkinnedMesh.h"
#include "DeferredGraphics.h"
#include "Texture.h"
#include "ModelResource.h"

class EntityDataAsset
{
public:
	EntityDataAsset(shared_ptr<DeferredGraphics> graphics);
	void	registerGeometry(std::string const& key, Model const& model);
	shared_ptr<SkinnedMesh>	getGeometry(std::string const& key);
	void	registerTexture(std::string const& key, std::wstring const& path);
	shared_ptr<Texture>	getTexture(std::string const& key);
private:
	ComPtr<ID3D11Device> device;
	std::map<std::string, shared_ptr<SkinnedMesh>> table;
	std::map<std::string, shared_ptr<Texture>> texture_table;
};