#pragma once

struct Transform
{
	vec3 position = {0, 0, 0};
	vec4 rotation = {0, 0, 0, 1};
	vec3 scale = {1, 1, 1};

	Mat	toMatrix() const;
};