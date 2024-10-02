#include "pch.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "CameraMove.h"
#include "model.h"


SceneSystem::SceneSystem(shared_ptr<Graphics> graphics, shared_ptr<ResourceSystem> resource_system, shared_ptr<InputSystem> input_system, shared_ptr<TimeSystem> time_system)
    :graphics(graphics), resource_system(resource_system), input_system(input_system), time_system(time_system)
{

}

void SceneSystem::Init()
{
    if (this->active_scene == nullptr)
        return;

    this->active_scene->Awake();
    this->active_scene->Start();
}

void SceneSystem::Update()
{
    if (this->active_scene == nullptr)
        return;
    this->active_scene->Update();
    this->active_scene->LateUpdate();
    this->active_scene->FixedUpdate();
}

void SceneSystem::LoadScene(wstring scene_name)
{
    this->active_scene = LoadMinecraftScene();
    Init();
}

shared_ptr<Scene> SceneSystem::LoadMinecraftScene()
{
    shared_ptr<Scene> scene = make_shared<Scene>();
    {
        shared_ptr<GameObject> camera = make_shared<GameObject>();
        camera->GetOrAddTransform()->SetPosition(vec3(0.f,0.f,-10.f));
        camera->GetOrAddTransform();
        camera->AddComponent(make_shared<Camera>(800.f, 600.f));
        camera->AddComponent(make_shared<CameraMove>(this->input_system, this->time_system, camera));
        scene->AddGameObject(camera);
    }
    {
        //shared_ptr<GameObject> steve = make_shared<GameObject>();
        //steve->GetOrAddTransform();
        //auto steve_mesh_renderer = make_shared<MeshRenderer>(this->graphics->getDevice());
        //auto steve_material = this->resource_system->Get<Material>(L"steve_material");
        //steve_mesh_renderer->SetMaterial(steve_material);
        //auto steve_mesh = this->resource_system->Get<Mesh>(L"steve");
        //steve_mesh_renderer->SetMesh(steve_mesh);
        //steve->AddComponent(steve_mesh_renderer);
        //scene->AddGameObject(steve);
        for (const auto& bone : model::humanoid_geometry.bone)
        {
            
            shared_ptr<GameObject> steve = make_shared<GameObject>();
            steve->GetOrAddTransform();
            auto steve_mesh_renderer = make_shared<MeshRenderer>(this->graphics->getDevice());
            auto steve_material = this->resource_system->Get<Material>(L"steve_material");
            steve_mesh_renderer->SetMaterial(steve_material);
            if (!bone.BoxCheck)
                continue;
            auto steve_mesh = this->resource_system->Get<Mesh>(bone.name);
            steve_mesh_renderer->SetMesh(steve_mesh);
            for (const auto& shape : bone.shape)
            {
                steve->GetOrAddTransform()->SetLocalPosition(shape.origin / 16);
            }
            steve->AddComponent(steve_mesh_renderer);
            scene->AddGameObject(steve);
        }
    }
    return scene;
}
