#pragma once

#include "InputLayouts.h"
#include "DeferredRendering.h"
#include "EntityReigstry.h"
#include "ChunkMeshRegistry.h"
//#include "ChunkRegistry.h"

#include "RenderingContext.h"

#include "RenderPipeline.h"

class Graphics;

class RenderSystem
{
public:
	//RenderSystem(MapUtils* minfo, EntityUtils* einfo);
	RenderSystem(TextureRegistry& texture_registry, EntityRegistry& entity_registry, ChunkMeshRegistry& chunk_mesh_registry, Graphics& graphics, int width, int height);
	//void setDeffGraphics(DeferredGraphics* defer_graphic);
	//void Render(
	//	Mat const& cam_view,
	//	Mat const& cam_proj,
	//	vec3 const& cam_pos,
	//	Mat const& shadow_view,
	//	Mat const& shadow_proj
	//);
	void	update();
private:

	TextureRegistry& texture_registry;
	EntityRegistry& entity_registry;
	ChunkMeshRegistry& chunk_mesh_registry;
	Graphics& graphics;
	RenderingContext context;
	RenderPipeline render_pipeline;

	//MapUtils* m_info;
	//EntityUtils* e_info;
	//shared_ptr<DeferredRendering> r_deff;
};

