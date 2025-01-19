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
