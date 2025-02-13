#pragma once

#include "RenderPipeline.h"

class DirectionalShadowCaster
{
public:
	DirectionalShadowCaster(RenderingContext& context, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry);

	void render();
private:
	RenderPipeline render_pipeline;
};
