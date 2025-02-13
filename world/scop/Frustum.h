#pragma once

struct Frustum
{
	vec4 left_plane;
	vec4 right_plane;
	vec4 top_plane;
	vec4 bottom_plane;
	vec4 near_plane;
	vec4 far_plane;

	bool intersectsSphere(vec3 center, float radius) const;
};