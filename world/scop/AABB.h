#pragma once

struct AABB
{
	vec3 center;
	vec3 extents;

	bool	isInsideExclusive(vec3 point) const;
	bool	isInsideInclusive(vec3 point) const;
};