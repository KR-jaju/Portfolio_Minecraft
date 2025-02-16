#pragma once

#include "InputLayouts.h"
#include "DeferredRendering.h"
#include "EntityReigstry.h"
#include "ChunkRegistry.h"

#include "MainCamera.h"

#include "RenderingContext.h"

class Graphics;

class RenderSystem
{
public:
	RenderSystem(ThreadPool& thread_pool, Camera& camera, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, Graphics& graphics, int width, int height);

	void	update();
private:
	Camera& camera;
	AssetManager& asset_manager;
	LightRegistry& light_registry;
	EntityRegistry& entity_registry;
	ChunkRegistry& chunk_registry;
	Graphics& graphics;

	RenderingContext context;
	MainCamera main_camera; // Main Camera
};

