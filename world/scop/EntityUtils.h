#pragma once

#include <vector>
#include "Entity.h"

class ConstantBuffer;

class EntityUtils
{
public:
	EntityUtils();

	void	init(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);

	uint32	addEntity(Entity&& entity);
	Entity& getEntity(uint32 id);
	void	removeEntity(uint32 id);

	void	render(ComPtr<ID3D11DeviceContext> context);
	void	renderShadowMap(ComPtr<ID3D11DeviceContext> context);
private:
	std::vector<Entity> entity_list;
	std::vector<uint32> free_id;
	std::vector<bool> is_valid_entity;
	shared_ptr<ConstantBuffer> bone_transform_cb;
};