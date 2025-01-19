#include "pch.h"
#include "PhysicsSystem.h"
#include <algorithm>
#include <limits>

PhysicsSystem::PhysicsSystem(BlockDataRegistry& block_data_registry, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry)
	: block_data_registry(block_data_registry),
	entity_registry(entity_registry),
	chunk_registry(chunk_registry)
{

}

void	PhysicsSystem::update(float dt)
{
	std::vector<AABB> candidates;

	for (Entity& entity : this->entity_registry)
	{
		AABB entity_aabb = entity.getBoundingBox();
		vec3 position = entity.getPosition();
		vec3 velocity = entity.getVelocity();
		int contact_faces = DIRECTION_CLEAR_BIT;
		float dt_left = dt;
		SweepResult result = {};

		candidates.clear();
		this->broadPhase(position, entity_aabb, velocity * dt_left, candidates); // 주변 블럭들 가져오기
		while (true) // 후보에서 스윕 테스트
		{
			if (this->aabbcast(position, entity_aabb, velocity * dt_left, candidates, result))
			{
				float dt_past = dt_left * result.t;

				position += velocity * dt_past + result.normal * 0.001f;
				velocity -= result.normal * velocity.Dot(result.normal); // 속도에서 노말방향 성분을 전부 제거 (바닥에 닿지 않으면 y성분은 유지가 돼야함.)
				contact_faces |= (1 << static_cast<int>(result.direction));
				dt_left -= dt_past;
			}
			else
			{
				position += velocity * dt_left;
				break;
			}
		}	
		//entity.setVelocity((position - entity.getPosition()) / dt); // 최종적으로 변한 속도
		entity.setVelocity(velocity);
		entity.setPosition(position);
		entity.setContactFaces(contact_faces);
	}
}

void	PhysicsSystem::broadPhase(vec3 position, AABB const& entity_aabb, vec3 displacement, std::vector<AABB>& candidates)
{
	ChunkRegistry& chunk_registry = this->chunk_registry;
	AABB boundary = { entity_aabb.center + position + displacement * 0.5f, entity_aabb.extents + vec3(std::abs(displacement.x), std::abs(displacement.y), std::abs(displacement.z))}; // 체크해야하는 영역
	vec3 min = boundary.center - boundary.extents;
	vec3 max = boundary.center + boundary.extents;

	min.x = std::floor(min.x);
	min.y = std::floor(min.y);
	min.z = std::floor(min.z);
	max.x = std::ceil(max.x);
	max.y = std::ceil(max.y);
	max.z = std::ceil(max.z);
	for (int y = static_cast<int>(min.y) - 1; y <= static_cast<int>(max.y); ++y)
	{
		for (int z = static_cast<int>(min.z) - 1; z <= static_cast<int>(max.z); ++z)
		{
			for (int x = static_cast<int>(min.x) - 1; x <= static_cast<int>(max.x); ++x)
			{
				BlockData block = chunk_registry.getBlock(x, y, z);
				std::vector<AABB> const& aabb_list = block_data_registry.getShape(block);

				for (AABB const& aabb : aabb_list)
				{
					candidates.push_back({ aabb.center + vec3(x, y, z), aabb.extents });
				}
			}
		}
	}
}

bool	PhysicsSystem::aabbcast(vec3 position, AABB const& aabb, vec3 displacement, std::vector<AABB> const& candidates, SweepResult& result_out)
{
	AABB box = { aabb.center + position, aabb.extents };
	SweepResult best_result = {
		vec3::Zero,
		std::numeric_limits<float>::infinity(),
		Direction::Undefined
	};

	for (AABB const& candidate : candidates)
	{
		SweepResult result;

		if (this->sweepTest(box, candidate, displacement, result))
		{
			if (result.t < best_result.t)
				best_result = result;
		}
	}
	if (best_result.t == std::numeric_limits<float>::infinity())
		return (false);
	result_out = best_result;
	return (true);
}


bool	PhysicsSystem::sweepTest(AABB const& a, AABB const& b, vec3 displacement, SweepResult& result)
{
	float x_entry;
	float y_entry;
	float z_entry; // x, y, z 축이 겹치기 시작하는 순간
	float x_exit;
	float y_exit;
	float z_exit; // x, y, z 축이 겹치지 않기 시작하는 순간

	if (displacement.x == 0)
	{
		if (a.center.x - a.extents.x < b.center.x + b.extents.x && a.center.x + a.extents.x > b.center.x - b.extents.x)
			x_entry = -std::numeric_limits<float>::infinity();
		else
			x_entry = std::numeric_limits<float>::infinity();
		x_exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		x_entry = ((b.center.x - b.extents.x) - (a.center.x + a.extents.x)) / displacement.x;
		x_exit = ((b.center.x + b.extents.x) - (a.center.x - a.extents.x)) / displacement.x;
		if (displacement.x < 0)
			std::swap(x_entry, x_exit);
	}
	if (displacement.y == 0)
	{
		if (a.center.y - a.extents.y < b.center.y + b.extents.y && a.center.y + a.extents.y > b.center.y - b.extents.y)
			y_entry = -std::numeric_limits<float>::infinity();
		else
			y_entry = std::numeric_limits<float>::infinity();
		y_exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		y_entry = ((b.center.y - b.extents.y) - (a.center.y + a.extents.y)) / displacement.y;
		y_exit = ((b.center.y + b.extents.y) - (a.center.y - a.extents.y)) / displacement.y;
		if (displacement.y < 0)
			std::swap(y_entry, y_exit);
	}
	if (displacement.z == 0) // 속도가 0이라면 
	{
		if (a.center.z - a.extents.z < b.center.z + b.extents.z && a.center.z + a.extents.z > b.center.z - b.extents.z)
			z_entry = -std::numeric_limits<float>::infinity();
		else
			z_entry = std::numeric_limits<float>::infinity();
		z_exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		z_entry = ((b.center.z - b.extents.z) - (a.center.z + a.extents.z)) / displacement.z;
		z_exit = ((b.center.z + b.extents.z) - (a.center.z - a.extents.z)) / displacement.z;
		if (displacement.z < 0)
			std::swap(z_entry, z_exit);
	}
	if ((x_entry < 0.0f && y_entry < 0.0f && z_entry < 0.0f) || x_entry > 1.0f || y_entry > 1.0f || z_entry > 1.0f) // 모든 축이 겹쳐있거나(이미 내부임) 적어도 하나의 축이 겹치지 않을 때는 실패
		return (false);
	float exit_time = std::min<float>(x_exit, std::min<float>(y_exit, z_exit));

	if (x_entry > y_entry && x_entry > z_entry) // 가장 마지막에 겹치기 시작하는 축이
	{
		if (x_entry > exit_time) // 가장 먼저 안 겹치기 시작한 축보다 늦게 겹치면(세 축이 동시에 겹치는 경우가 없음)
			return (false);
		bool neg = std::signbit(displacement.x);

		result.normal = vec3(neg ? 1 : -1, 0, 0);
		result.t = x_entry;
		result.direction = (neg) ? Direction::West : Direction::East;
	}
	else if (y_entry > z_entry)
	{
		if (y_entry > exit_time)
			return (false);
		bool neg = std::signbit(displacement.y);

		result.normal = vec3(0, neg ? 1 : -1, 0);
		result.t = y_entry;
		result.direction = (neg) ? Direction::Down : Direction::Up;
	}
	else 
	{	
		if (z_entry > exit_time)
			return (false);
		bool neg = std::signbit(displacement.z);

		result.normal = vec3(0, 0, neg ? 1 : -1);
		result.t = z_entry;
		result.direction = neg ? Direction::South : Direction::North;
	} // 모든 축이 다 맞아야 들어간거니까 가장 큰걸 기준으로 봄
	return (true);
}


//void	PhysicsSystem::narrowPhase(AABB const& entity_aabb, std::vector<AABB> const& candidates, std::vector<CollisionInfo>& collisions)
//{
//	for (AABB const& collider : candidates)
//	{
//		vec3 closest_point = entity_aabb.center;
//
//		closest_point.x = std::clamp(closest_point.x, collider.center.x - collider.extents.x, collider.center.x + collider.extents.x);
//		closest_point.y = std::clamp(closest_point.y, collider.center.y - collider.extents.y, collider.center.y + collider.extents.y);
//		closest_point.z = std::clamp(closest_point.z, collider.center.z - collider.extents.z, collider.center.z + collider.extents.z);
//
//		float dx = closest_point.x - entity_aabb.center.x;
//		float dy = closest_point.y - entity_aabb.center.y;
//		float dz = closest_point.z - entity_aabb.center.z;
//
//		float x_overlap = entity_aabb.extents.x - abs(dx);
//		float y_overlap = entity_aabb.extents.y - abs(dy);
//		float z_overlap = entity_aabb.extents.z - abs(dz);
//
//		CollisionInfo info;
//
//		if (!entity_aabb.isInsideExclusive(closest_point))
//			continue;
//		if (x_overlap < y_overlap && x_overlap < z_overlap) {
//			info.contact_point = closest_point;
//			info.normal = (dx < 0) ? vec3(1, 0, 0) : vec3(-1, 0, 0);
//			info.overlap = x_overlap;
//		}
//		else if (y_overlap < z_overlap) {
//			info.contact_point = closest_point;
//			info.normal = (dy < 0) ? vec3(0, 1, 0) : vec3(0, -1, 0);
//			info.overlap = y_overlap;
//		}
//		else {
//			info.contact_point = closest_point;
//			info.normal = (dz < 0) ? vec3(0, 0, 1) : vec3(0, 0, -1);
//			info.overlap = z_overlap;
//		}
//
//		collisions.push_back(info);
//	}
//}
//
//void	PhysicsSystem::resolveCollision(Entity& entity, std::vector<CollisionInfo> const& collisions)
//{
//	for (CollisionInfo const& info : collisions)
//	{
//		AABB entity_aabb = entity.getBoundingBox();
//
//		entity_aabb.center += entity.getPosition();
//		if (!entity_aabb.isInsideExclusive(info.contact_point))
//			continue;
//		entity.setPosition(entity.getPosition() + info.normal * info.overlap);
//		vec3 velocity = entity.getVelocity();
//
//		velocity -= info.normal * velocity.Dot(info.normal);
//		entity.setVelocity(velocity);
//	}
//}