#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "Camera.h"
#include "Transform.h"
#include "MonoBehaviour.h"
#include "MeshRenderer.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : this->components)
	{
		if (component)
			component->Awake();
	}
	for (shared_ptr<MonoBehaviour>& script : this->scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : this->components)
	{
		if (component)
			component->Start();
	}
	for (shared_ptr<MonoBehaviour>& script : this->scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : this->components)
	{
		if (component)
			component->Update();
	}
	for (shared_ptr<MonoBehaviour>& script : this->scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : this->components)
	{
		if (component)
			component->LateUpdate();
	}
	for (shared_ptr<MonoBehaviour>& script : this->scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : this->components)
	{
		if (component)
			component->FixedUpdate();
	}
	for (shared_ptr<MonoBehaviour>& script : this->scripts)
	{
		script->FixedUpdate();
	}
}
shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return this->components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Transform);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Camera);
	return static_pointer_cast<Camera>(component);
}
shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::MeshRenderer);
	return dynamic_pointer_cast<MeshRenderer>(component);
}


shared_ptr<Transform> GameObject::GetOrAddTransform()
{
	if (GetTransform() == nullptr)
	{
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	return GetTransform();
}


void GameObject::AddComponent(shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());
	
	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		this->components[index] = component;
	}
	else
	{
		this->scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}


