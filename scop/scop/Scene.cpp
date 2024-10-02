#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
void Scene::Awake()
{
	for (const shared_ptr<GameObject>& game_object : this->game_objects)
	{
		game_object->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& game_object : this->game_objects)
	{
		game_object->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& game_object : this->game_objects)
	{
		game_object->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& game_object : this->game_objects)
	{
		game_object->LateUpdate();
	}
}

void Scene::FixedUpdate()
{
	for (const shared_ptr<GameObject>& game_object : this->game_objects)
	{
		game_object->FixedUpdate();
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> game_object)
{
	this->game_objects.push_back(game_object);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> game_object)
{
	auto findIt = std::find(this->game_objects.begin(), this->game_objects.end(), game_object);
	if (findIt != this->game_objects.end())
		this->game_objects.erase(findIt);
}
