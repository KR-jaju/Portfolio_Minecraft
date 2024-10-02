#pragma once
#include "Graphics.h"
#include "Pipeline.h"
#include "RenderData.h"
#include "ConstantBuffer.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "GameObject.h"

class SceneSystem;
class RenderSystem
{
public:
	RenderSystem(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> device_context, shared_ptr<SceneSystem> scene_system);

	void Init();
	void Update(shared_ptr<Graphics> graphicss);

private:
	void PushCameraData();
	void PushTransformData();
	void PushAnimationData();

	void GatherRenderableObjects();
	void RenderObjects();

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> device_context;
	shared_ptr<Pipeline> pipeline;
	shared_ptr<SceneSystem> scene_system;
private:
	CameraData camera_data;
	shared_ptr<ConstantBuffer> camera_buffer;

	TransformData transform_data;
	shared_ptr<ConstantBuffer> transform_buffer;

private:
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<BlendState> blend_state;

private:
	vector<shared_ptr<GameObject>> render_objects;
	
};

