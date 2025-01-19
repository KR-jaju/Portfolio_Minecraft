#include "pch.h"
#include "RenderSystem.h"
#include "GeometryPass.h"
#include "Lightingpass.h"
#include "ToneMappingPass.h"

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

//static Mat createViewMatrix(Player const& player)
//{
//	vec3 position = player.getPosition();
//	vec3 rotation = player.getRotation();	
//	Mat inv_r = Mat::CreateFromYawPitchRoll(rotation.y + 3.141592f, rotation.x, rotation.z).Transpose(); // 180도 더하는건 z+가 앞으로 가게 하기 위함임!
//	Mat inv_t = Mat::CreateTranslation(-position.x, -position.y - 1.6f, -position.z);
//
//	return (inv_t * inv_r);
//}
//
//static Mat createProjectionMatrix(Player const& player, float aspect_ratio)
//{
//	float fov = player.getFov();
//
//	return Mat::CreatePerspectiveFieldOfView(fov, aspect_ratio, 0.3f, 500.0f);
//}

void	RenderSystem::update()
{
	Player& player = this->entity_registry.getPlayer();
	Camera const& camera = player.getCamera();

	float const aspect_ratio = this->context.viewport_width / this->context.viewport_height;
	//Mat const view_matrix = createViewMatrix(player);
	//Mat const projection_matrix = createProjectionMatrix(player, aspect_ratio);
	Mat const view_matrix = camera.getViewMatrix();
	Mat const projection_matrix = camera.getProjectionMatrix();

	this->context.camera_data.update((view_matrix * projection_matrix).Transpose());
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
