#pragma once
#include "pch.h"

namespace model
{
	struct Box {
		vec3 origin;
		vec3 size;
		vec2 uv;
	};
	struct Bone {
		wstring name;
		wstring parent;
		vec3 pivot;
		vector<Box> shape;
		bool BoxCheck;
	};
	struct Geometry {
		vector<Bone> bone;
	};
	extern Geometry humanoid_geometry;
}
