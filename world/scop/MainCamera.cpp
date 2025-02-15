#include "pch.h"
#include "MainCamera.h"

MainCamera::MainCamera(ThreadPool& thread_pool, Graphics& graphics, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, RenderingContext& context)
	: graphics(graphics),
	player(entity_registry.getPlayer()),
	camera_matrices(),
	camera_data(graphics.getDevice(), graphics.getContext(), this->camera_matrices),
	geometry_pass(thread_pool),
	lighting_pass(thread_pool),
	tone_mapping_pass(thread_pool)
{
	ComPtr<ID3D11Device> device = graphics.getDevice();

	this->geometry_pass.initialize(device, asset_manager);
	this->lighting_pass.initialize(device, asset_manager);
	this->tone_mapping_pass.initialize(device, asset_manager);
}
/*
OcclusionCulling -> Frustum Culling

GeometryPass -> OcclusionCulling
ShadowPass -> shadow의 OcclusionCulling

Lighting Pass -> GeometryPass, ShadowPass
SkyPass -> Lighting Pass
ToneMappingPass -> SkyPass
Pass의 결과는 누군가는 get을 해서 받아야함.(std::shared_future) 쓰면 될듯

1. VS,PS의 0번 CB는 메인 카메라의 행렬로 고정함.
*/

void MainCamera::render(Camera& camera, RenderGroup const& render_group)
{
	//RenderGroup const& result = this->visibility
	//	.intialize(render_group, camera)
	//	.applyFrustumCulling()
	//	.applyOcclusionCulling(this->past_render_group)
	//	.getVisibleObjects();
	//ComPtr<ID3D11DeviceContext> dc = this->graphics.getContext();

	int width = 800;
	int height = 800;

	float const aspect_ratio = width / height;
	CameraMatrices& vp = this->camera_matrices;

	vp.view = camera.getViewMatrix();
	vp.projection = camera.getProjectionMatrix();
	vp.view_projection = vp.view * vp.projection;
	vp.view_inverse_transpose = camera.getViewInverseTransposeMatrix();
	vp.projection_inverse = camera.getProjectionInverseMatrix();
	vp.dimension = ivec4(width, height, 0, 0);


	//this->render_pipeline.render(result); // 렌더;
	ComPtr<ID3D11DeviceContext> immediate_context = graphics.getContext();
	//std::mutex context_mutex;

	//std::shared_future<void> visibility_ready = this->visibility.process(render_group, this->past_render_group);
	//std::shared_future<void> geometry_ready = this->geometry_pass.dispatch(immediate_context, context_mutex, visibility_ready, this->visibility.getResources());
	//std::shared_future<void> lighting_ready = this->lighting_pass.dispatch(immediate_context, context_mutex, geometry_ready, this->geometry_pass.getResources());
	//std::shared_future<void> sky_ready = this->sky_pass.dispatch(immediate_context, context_mutex, lighting_ready, this->lighting_pass.getResources());
	//std::shared_future<void> tone_mapping_result = this->tone_mapping_pass.dispatch(immediate_context, context_mutex, sky_ready, this->sky_pass.getResources());

	this->camera_data.update(vp);
	immediate_context->VSSetConstantBuffers(0, 1, this->camera_data.getComPtr().GetAddressOf());
	immediate_context->PSSetConstantBuffers(0, 1, this->camera_data.getComPtr().GetAddressOf());

	this->visibility.process(immediate_context, render_group, camera.getFrustum(), this->past_render_group);
	this->geometry_pass.execute(immediate_context, this->visibility.getResult());
	this->lighting_pass.execute(immediate_context, this->geometry_pass.getResources());
	this->tone_mapping_pass.execute(immediate_context, this->lighting_pass.getResources());

	ComPtr<ID3D11Texture2D> swap_chain_texture = this->graphics.getSwapChainTexture();

	immediate_context->CopyResource(swap_chain_texture.Get(), this->tone_mapping_pass.getResources().texture.Get());
}