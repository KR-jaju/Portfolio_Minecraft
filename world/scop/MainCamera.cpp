#include "pch.h"
#include "MainCamera.h"

MainCamera::MainCamera(ThreadPool& thread_pool, Graphics& graphics, AssetManager& asset_manager, LightRegistry& light_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry, RenderingContext& context)
	: graphics(graphics),
	player(entity_registry.getPlayer()),
	geometry_pass(thread_pool),
	lighting_pass(thread_pool),
	tone_mapping_pass(thread_pool)
{
	ComPtr<ID3D11Device> device = graphics.getDevice();

	{// CB
		ComPtr<ID3D11Buffer> subchunk_cb;
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA data = {};

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Camera::Info); // (x, y, z, padding)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = device->CreateBuffer(&desc, nullptr, this->camera_info.GetAddressOf());
		CHECK(hr);
	}
	this->visibility.initialize(device, asset_manager, 800, 800);
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
	//CameraMatrices& vp = this->camera_matrices;

	//vp.view = camera.getViewMatrix();
	//vp.projection = camera.getProjectionMatrix();
	//vp.view_projection = vp.view * vp.projection;
	//vp.view_inverse_transpose = camera.getViewInverseTransposeMatrix();
	//vp.projection_inverse = camera.getProjectionInverseMatrix();
	//vp.dimension = ivec4(width, height, 0, 0);


	//this->render_pipeline.render(result); // 렌더;
	ComPtr<ID3D11DeviceContext> immediate_context = graphics.getContext();
	//std::mutex context_mutex;

	//std::shared_future<void> visibility_ready = this->visibility.process(render_group, this->past_render_group);
	//std::shared_future<void> geometry_ready = this->geometry_pass.dispatch(immediate_context, context_mutex, visibility_ready, this->visibility.getResources());
	//std::shared_future<void> lighting_ready = this->lighting_pass.dispatch(immediate_context, context_mutex, geometry_ready, this->geometry_pass.getResources());
	//std::shared_future<void> sky_ready = this->sky_pass.dispatch(immediate_context, context_mutex, lighting_ready, this->lighting_pass.getResources());
	//std::shared_future<void> tone_mapping_result = this->tone_mapping_pass.dispatch(immediate_context, context_mutex, sky_ready, this->sky_pass.getResources());

	//this->camera_data.update(camera.getInfo());
	D3D11_MAPPED_SUBRESOURCE subresource = {};
	HRESULT hr = immediate_context->Map(this->camera_info.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	CHECK(hr);
	Camera::Info& info = *reinterpret_cast<Camera::Info*>(subresource.pData);
	info = camera.getInfo();
	immediate_context->Unmap(this->camera_info.Get(), 0);

	immediate_context->VSSetConstantBuffers(0, 1, this->camera_info.GetAddressOf());
	immediate_context->PSSetConstantBuffers(0, 1, this->camera_info.GetAddressOf());

	this->visibility.process(immediate_context, render_group, camera, this->past_render_group, this->camera_info);
	this->visibility.getResult(this->past_render_group);
	this->geometry_pass.execute(immediate_context, this->past_render_group, this->camera_info);
	this->lighting_pass.execute(immediate_context, this->geometry_pass.getResources(), this->camera_info);
	this->tone_mapping_pass.execute(immediate_context, this->lighting_pass.getResources());

	ComPtr<ID3D11Texture2D> swap_chain_texture = this->graphics.getSwapChainTexture();

	immediate_context->CopyResource(swap_chain_texture.Get(), this->tone_mapping_pass.getResources().texture.Get());
}