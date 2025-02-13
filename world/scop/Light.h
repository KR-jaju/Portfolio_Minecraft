#pragma once

#include "Camera.h"

class DirectionalLight
{
public:
	DirectionalLight();

	void	update(Camera const& camera);

	vec3 getDirection() const;
	vec3 getColor() const;
	Mat const& getViewProjectionMatrix(int level) const;
private:
	Mat view_projection[4];
	vec3 direction;
	vec3 color;
};

struct DirectionalLightData
{
	Mat view_projection[4];
	vec3 incoming_direciton;
	float padding0;
	vec3 color;
	float padding1;
};

struct alignas(16) PointLight
{
	vec4 position;
	vec4 color; // color(rgb) + bias
	vec2 near_far;
};
