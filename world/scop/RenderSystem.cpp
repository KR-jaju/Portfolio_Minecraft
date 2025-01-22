#include "pch.h"
#include "RenderSystem.h"
#include "GeometryPass.h"
#include "Lightingpass.h"
#include "ToneMappingPass.h"
#include "CameraMatrices.h"
#include "Player.h"

RenderSystem::RenderSystem(TextureRegistry& texture_registry, EntityRegistry& entity_registry, ChunkMeshRegistry& chunk_mesh_registry, Graphics& graphics, int width, int height)
	:  texture_registry(texture_registry),
	entity_registry(entity_registry),
	chunk_mesh_registry(chunk_mesh_registry),
	graphics(graphics),
	context(texture_registry, entity_registry, chunk_mesh_registry, graphics, width, height),
	render_pipeline(texture_registry, graphics, context)
{
	this->render_pipeline.addPass<GeometryPass>();
	this->render_pipeline.addPass<LightingPass>();
	this->render_pipeline.addPass<ToneMappingPass>();
}

void	RenderSystem::update()
{
	Player& player = this->entity_registry.getPlayer();
	Camera const& camera = player.getCamera();

	float const aspect_ratio = this->context.viewport_width / this->context.viewport_height;
	CameraMatrices vp;

	vp.view = camera.getViewMatrix().Transpose();
	vp.projection = camera.getProjectionMatrix().Transpose();
	vp.view_projection = vp.projection * vp.view;
	vp.view_inverse_transpose = camera.getViewInverseMatrix().Transpose();
	vp.projection_inverse = vp.projection.Transpose().Invert().Transpose();

	this->context.camera_data.update(vp);
	this->render_pipeline.render();
	this->graphics.present();
}

//void RenderSystem::setDeffGraphics(DeferredGraphics* defer_graphic)
//{
//	this->r_deff = make_shared<DeferredRendering>(
//		this->m_info,
//		this->e_info,
//		defer_graphic
//	);
//}
//
//void RenderSystem::Render(
//	Mat const& cam_view, 
//	Mat const& cam_proj,
//	vec3 const& cam_pos,
//	Mat const& shadow_view,
//	Mat const& shadow_proj
//)
//{
//	this->r_deff->Render(cam_view, cam_proj, cam_pos, shadow_view, shadow_proj);
//}

// 1. shadow rendering
// 2. geometry pass
// 3. lighting pass
// 4. post processing
