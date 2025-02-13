#include "pch.h"
#include "DirectionalShadowCaster.h"

DirectionalShadowCaster::DirectionalShadowCaster(RenderingContext& context, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry)
	: render_pipeline(context, asset_manager)
{

}

void	DirectionalShadowCaster::render()
{

}
