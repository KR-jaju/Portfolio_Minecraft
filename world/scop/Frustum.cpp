#include "pch.h"
#include "Frustum.h"

static float signedDistance(vec4 plane, vec3 point)
{
	float dot = plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
	float mag = std::sqrtf(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);

	return (dot / mag);
}

bool	Frustum::intersectsSphere(vec3 center, float radius) const
{
	return (
		signedDistance(this->left_plane, center) < radius &&
		signedDistance(this->right_plane, center) < radius &&
		signedDistance(this->top_plane, center) < radius &&
		signedDistance(this->bottom_plane, center) < radius &&
		signedDistance(this->near_plane, center) < radius &&
		signedDistance(this->far_plane, center) < radius
		);
}
