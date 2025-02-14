#include "pch.h"
#include "MainCamera.h"

#include "ShadowPass.h"
#include "GeometryPass.h"
#include "Lightingpass.h"
#include "SkyPass.h"
#include "ToneMappingPass.h"

MainCamera::MainCamera(Graphics& graphics, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, RenderingContext& context)
	: graphics(graphics),
	player(entity_registry.getPlayer()),
	camera_matrices(),
	camera_data(graphics.getDevice(), graphics.getContext(), this->camera_matrices),
	render_pipeline(context, asset_manager) //TODO: 순서를 명확하게
{
	this->render_pipeline.addPass(make_unique<GeometryPass>());
	this->render_pipeline.addPass(make_unique<LightingPass>());
	this->render_pipeline.addPass(make_unique<SkyPass>());
	this->render_pipeline.addPass(make_unique<ToneMappingPass>());
}
/*
1. VS,PS의 0번 CB는 메인 카메라의 행렬로 고정함.
*/

void MainCamera::render(Camera& camera, RenderGroup const& render_group)
{
	VisibilityProcessor visibility(render_group);
	RenderGroup const& result = visibility
		.applyFrustumCulling(camera.getFrustum())
		.getVisibleObjects();
	ComPtr<ID3D11DeviceContext> dc = this->graphics.getContext();

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

	this->camera_data.update(vp);
	dc->VSSetConstantBuffers(0, 1, this->camera_data.getComPtr().GetAddressOf());
	dc->PSSetConstantBuffers(0, 1, this->camera_data.getComPtr().GetAddressOf());

	this->render_pipeline.render(result); // 렌더;
}