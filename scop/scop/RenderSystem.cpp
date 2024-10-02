#include "pch.h"
#include "RenderSystem.h"
#include "SceneSystem.h"
#include "RenderData.h"
#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "model.h"

RenderSystem::RenderSystem(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> device_context, shared_ptr<SceneSystem> scene_system)
	:device(device), device_context(device_context), scene_system(scene_system)
{
}

void RenderSystem::Init()
{
	this->pipeline = make_shared<Pipeline>(this->device_context);
	this->camera_buffer = make_shared<ConstantBuffer>(this->device, this->device_context, this->camera_data);
	this->transform_buffer = make_shared<ConstantBuffer>(this->device, this->device_context, this->transform_data);
}

void RenderSystem::Update(shared_ptr<Graphics> graphics)
{
	graphics->renderBegin();

	GatherRenderableObjects();
	RenderObjects();
	
	graphics->renderEnd();
}

void RenderSystem::PushCameraData()
{
	this->camera_buffer->update(this->camera_data);
}

void RenderSystem::PushTransformData()
{
	this->transform_buffer->update(this->transform_data);
}

void RenderSystem::PushAnimationData()
{

}

void RenderSystem::GatherRenderableObjects()
{
	this->render_objects.clear();

	auto& game_objects = this->scene_system->GetActiveScene()->GetGameObjects();
	for (const shared_ptr<GameObject>& game_object : game_objects)
	{
		shared_ptr<MeshRenderer> g_mesh_renderer = game_object->GetMeshRenderer();
		if (g_mesh_renderer)
			render_objects.push_back(game_object);
		shared_ptr<Camera> camera = game_object->GetCamera();
		if (camera)
		{
			camera_data.mat_projection = camera->GetProjectionMatrix().Transpose();
			camera_data.mat_view = camera->GetViewMatrix().Transpose();
			PushCameraData();
		}
	}
}

void RenderSystem::RenderObjects()
{
	for (const shared_ptr<GameObject>& game_object : render_objects)
	{
		shared_ptr<MeshRenderer> mesh_renderer = game_object->GetMeshRenderer();
		if (mesh_renderer == nullptr)
			continue;
		
		for (const auto& bone : model::humanoid_geometry.bone)
		{
			if (mesh_renderer->GetMesh()->GetName() == bone.name)
			{
				auto& game_objects = this->scene_system->GetActiveScene()->GetGameObjects();
				for (const shared_ptr<GameObject>& game_object : game_objects)
				{
					if (game_object->GetMeshRenderer())
					{
						if (game_object->GetMeshRenderer()->GetMesh()->GetName() == bone.parent)
							mesh_renderer->GetTransform()->SetParent(game_object->GetTransform());
					}
					else
						continue;
				}
			}
			else
				continue;
		}

		shared_ptr<Transform> transform = game_object->GetTransform();
		if (transform == nullptr)
			continue;
		//SRT
		this->transform_data.mat_world = transform->GetWorldMatrix();
		PushTransformData();
		PipelineInfo info;
		info.input_layout = mesh_renderer->GetInputLayout();
		info.vertex_shader = mesh_renderer->GetVertexShader();
		info.pixel_shader = mesh_renderer->GetPixelShader();
		info.rasterizer_state = this->rasterizer_state;
		info.blend_state = this->blend_state;
		this->pipeline->UpdatePipeline(info);

		this->pipeline->SetVertexBuffer(mesh_renderer->GetMesh()->GetVertexBuffer());
		this->pipeline->SetIndexBuffer(mesh_renderer->GetMesh()->GetIndexBuffer());

		this->pipeline->SetVSConstantBuffer(0, camera_buffer);

		this->pipeline->SetVSConstantBuffer(1, transform_buffer);

		this->pipeline->SetPSTexture(0, mesh_renderer->GetTexture());

		this->pipeline->DrawIndexed(mesh_renderer->GetMesh()->GetIndexBuffer()->getCount(), 0, 0);
	}
}
