#pragma once

#include <vector>
#include "Asset.h"
#include "AnimationCurve.h"
#include "Armature.h"

class AnimationClip : public Asset
{
public:
	AnimationClip(std::wstring const& path);
	void	sample(Armature& armature, float t, float weight = 1.0f, bool additive = false) const;
	float	getLength() const;
private:
	std::vector<AnimationCurve> curves;
	float length;
};