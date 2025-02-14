#pragma once

#include "RenderPipeline.h"
#include "VisibilityProcessor.h"
#include "RenderGroup.h"

class MainCamera
{
public:
	MainCamera(Graphics& graphics, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, RenderingContext& context);

	void render(Camera& camera, RenderGroup const& render_objects);
private:
	Graphics& graphics;
	Player const& player;

	CameraMatrices camera_matrices;
	ConstantBuffer camera_data;
	RenderPipeline render_pipeline;
};
