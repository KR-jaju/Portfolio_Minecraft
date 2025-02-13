#include "pch.h"
#include "AABB.h"

bool	AABB::isInsideExclusive(vec3 point) const
{
	float dx = std::abs(this->center.x - point.x);
	float dy = std::abs(this->center.y - point.y);
	float dz = std::abs(this->center.z - point.z);

	return (dx < this->extents.x && dy < this->extents.y && dz < this->extents.z);
}

bool	AABB::isInsideInclusive(vec3 point) const
{
	float dx = std::abs(this->center.x - point.x);
	float dy = std::abs(this->center.y - point.y);
	float dz = std::abs(this->center.z - point.z);

	return (dx <= this->extents.x && dy <= this->extents.y && dz <= this->extents.z);
}

void AABB::expand(vec3 point) {
    vec3 min_bound = center - extents;
    vec3 max_bound = center + extents;

    min_bound = vec3::Min(min_bound, point);
    max_bound = vec3::Max(max_bound, point);
    this->center = (min_bound + max_bound) * 0.5f;
    this->extents = (max_bound - min_bound) * 0.5f;
}
