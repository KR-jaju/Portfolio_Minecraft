#include "pch.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

Component::Component(ComponentType type)
    :type(type)
{
}

Component::~Component()
{
}

shared_ptr<Transform> Component::GetTransform()
{
    return this->game_object.lock()->GetTransform();
}

shared_ptr<GameObject> Component::GetGameObject()
{
    return this->game_object.lock();
}
