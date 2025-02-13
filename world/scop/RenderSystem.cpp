#include "pch.h"
#include "RenderSystem.h"
#include "CameraMatrices.h"
#include "Player.h"

RenderSystem::RenderSystem(AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, Graphics& graphics, int width, int height)
	: asset_manager(asset_manager),
	light_registry(light_registry),
	entity_registry(entity_registry),
	chunk_registry(chunk_registry),
	graphics(graphics),
	context(asset_manager, graphics, width, height),
	directional_shadow_caster(this->context, asset_manager, light_registry, entity_registry, chunk_registry),
	main_camera(graphics, asset_manager, light_registry, entity_registry, chunk_registry, this->context)
{
	
}

void	RenderSystem::update()
{
	ChunkRegistry& chunk_registry = this->chunk_registry;
	RenderGroup render_group;
	ivec2 const offset = chunk_registry.addressing_offset;
	int const render_distance = chunk_registry.addressing_half_stride - 1;
	int const render_stride = render_distance * 2 + 1;

	for (int z = offset.y - render_distance; z <= offset.y + render_distance; ++z)
	{
		for (int x = offset.x - render_distance; x <= offset.x + render_distance; ++x)
		{
			for (int y = 0; y < 16; ++y)
			{
				SubchunkMesh const& mesh = chunk_registry.getSubchunkMesh(x, y, z);

				render_group.add(x, y, z, mesh);
			}
		}
	}
	//this->directional_shadow_caster.render(render_group);
	this->main_camera.render(render_group);
	this->graphics.present();
}
