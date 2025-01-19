#pragma once

#include <vector>
#include "AnimationCurve.h"
#include "Armature.h"

class AnimationClip
{
public:
	void	sample(Armature& armature, float t, float weight = 1.0f, bool additive = false) const;
private:
	std::vector<AnimationCurve> curves;
};