#include "pch.h"
#include <vector>
#include "EntityUtils.h"
#include "ConstantBuffer.h"

EntityUtils::EntityUtils()
{

}

void	EntityUtils::init(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
{
	BoneData identity;

	this->bone_transform_cb = make_shared<ConstantBuffer>(device, context, identity);
}


uint32	EntityUtils::addEntity(Entity&& entity)
{
	if (this->free_id.empty())
	{
		uint32 id = this->entity_list.size();
		this->entity_list.push_back(std::move(entity));
		this->is_valid_entity.push_back(true);
		return (id);
	}
	uint32 id = this->free_id.back();
	this->free_id.pop_back();
	this->entity_list[id] = std::move(entity);
	this->is_valid_entity[id] = true;
	return (id);
}

Entity& EntityUtils::getEntity(uint32 id)
{
	return (this->entity_list[id]);
}

void	EntityUtils::removeEntity(uint32 id)
{
	this->is_valid_entity[id] = false;
	this->free_id.push_back(id);
}

void	EntityUtils::render(ComPtr<ID3D11DeviceContext> context)
{
	for (int id = 0; id < this->entity_list.size(); ++id)
	{
		if (!this->is_valid_entity[id])
			continue;
		Entity const& entity = this->entity_list[id];

		this->bone_transform_cb->update(entity.getBoneTransforms());
		context->VSSetConstantBuffers(1, 1, this->bone_transform_cb->getComPtr().GetAddressOf());
		entity.render(context);
	}
}
//void	EntityUtils::renderShadowMap();