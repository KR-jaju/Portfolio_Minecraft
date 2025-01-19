#pragma once

#include "BlockDataRegistry.h"
#include "EntityReigstry.h"
#include "ChunkRegistry.h"
#include "AABB.h"

class PhysicsSystem
{
public:
	PhysicsSystem(BlockDataRegistry& block_data_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry);

	void	update(float dt);
private:
	struct SweepResult
	{
		vec3 normal;
		float t;
		Direction direction;
	};

	BlockDataRegistry& block_data_registry;
	EntityRegistry& entity_registry;
	ChunkRegistry& chunk_registry;

	void	broadPhase(vec3 position, AABB const& aabb, vec3 displacement, std::vector<AABB>& candidates);
	bool	aabbcast(vec3 position, AABB const& aabb, vec3 displacement, std::vector<AABB> const& candidates, SweepResult& result);
	bool	sweepTest(AABB const& a, AABB const& b, vec3 displacement, SweepResult& result);
	//void	narrowPhase(AABB const& entity, std::vector<AABB> const& candidates, std::vector<CollisionInfo>& collisions);
	//void	resolveCollision(Entity& entity, std::vector<CollisionInfo> const& collisions);
	//struct CollisionInfo
	//{
	//	vec3 contact_point;
	//	vec3 normal;
	//	float overlap;
	//};
};
