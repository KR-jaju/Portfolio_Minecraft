#pragma once

struct AnimationCurve;
struct KeyFrame;
enum class WeightedMode;
enum class PropertyType;

struct AnimationClip
{
	std::vector<AnimationCurve> curves;
};

struct AnimationCurve
{
	PropertyType type;
	int bone_idx;
	std::vector<KeyFrame> keyframes;
};

struct KeyFrame
{
	float in_tangent;
	float in_weight;
	float out_tangent;
	float out_weight;
	float time;
	float value;
	WeightedMode weighted_mode;
};

enum class WeightedMode
{
	None,
	In,
	Out,
	Both
};

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

