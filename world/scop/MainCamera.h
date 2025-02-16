#pragma once

#include "VisibilityProcessor.h"
#include "RenderGroup.h"

#include "ShadowPass.h"
#include "GeometryPass.h"
#include "Lightingpass.h"
#include "ToneMappingPass.h"

class MainCamera
{
public:
	MainCamera(ThreadPool& thread_pool, Graphics& graphics, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, RenderingContext& context);

	void render(Camera& camera, RenderGroup const& render_objects);
private:
	Graphics& graphics;
	Player const& player;

	VisibilityProcessor visibility;
	RenderGroup	past_render_group;
	ComPtr<ID3D11Buffer> camera_info;

	GeometryPass geometry_pass;
	LightingPass lighting_pass;
	ToneMappingPass tone_mapping_pass;
};
