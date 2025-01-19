#include "pch.h"
#include "EntitySystem.h"
#include "Entity.h"

EntitySystem::EntitySystem(InputRegistry& input_registry, EntityRegistry& entity_registry)
	: input_registry(input_registry),
	entity_registry(entity_registry)
{

}

void EntitySystem::update(float dt)
{
	for (Entity& entity : this->entity_registry)
	{
		entity.update(dt);
	}
}