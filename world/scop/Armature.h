#pragma once

#include "Transform.h"

struct Armature
{
	Armature(size_t bone_count);
	std::vector<Transform> bone_transform;
};
