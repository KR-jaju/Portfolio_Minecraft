#pragma once

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
