#pragma once

#include <vector>

struct KeyFrame
{
	enum class WeightedMode
	{
		None,
		In,
		Out,
		Both
	};
	float in_tangent;
	float in_weight;
	float out_tangent;
	float out_weight;
	float time;
	float value;
	WeightedMode weighted_mode;
};

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