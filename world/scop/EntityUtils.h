#pragma once

#include <vector>
#include "Entity.h"

class ConstantBuffer;

class EntityUtils
{
public:
	EntityUtils();

	void	init(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);

	uint32	registerEntity(std::shared_ptr<Entity> const& entity);
	//uint32	addEntity(Entity&& entity);
	std::shared_ptr<Entity> getEntity(uint32 id);
	void	removeEntity(uint32 id);

	void	render(ComPtr<ID3D11DeviceContext> context);
	void	renderShadowMap(ComPtr<ID3D11DeviceContext> context);
private:
	std::vector<std::shared_ptr<Entity>> entity_list;
	std::vector<uint32> free_id;
	shared_ptr<ConstantBuffer> bone_transform_cb;
};