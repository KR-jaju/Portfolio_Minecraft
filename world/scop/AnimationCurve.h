#pragma once

#include <vector>
#include "KeyFrame.h"

struct AnimationCurve
{
	enum class PropertyType
	{
		PositionX,
		PositionY,
		PositionZ,
		RotationX,
		RotationY,
		RotationZ,
		RotationW,
		ScaleX,
		ScaleY,
		ScaleZ,
	};
	std::vector<KeyFrame> keyframes;
	int bone_idx;
	PropertyType type;

	float	evaluate(float t) const;
	bool	hasValue(float t) const;
};