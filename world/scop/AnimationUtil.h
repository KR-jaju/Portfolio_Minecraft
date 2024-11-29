#pragma once

#include "Transform.h"
#include "Animation.h"

class AnimationUtil
{
public:
	static void sample(Transform *dst, AnimationClip const& src, float t);
};
