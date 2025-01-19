#pragma once

struct EntityVertex
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 uv;
	vec4 weight;
	ivec4 bone;
};