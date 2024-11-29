#include "pch.h"
#include "AnimationUtil.h"
#include "Animation.h"
#include <algorithm>

static float interpolate(KeyFrame const& a, KeyFrame const& b, float t)
{
    // Normalize time to [0, 1] range
    float normalized_t = (t - a.time) / (b.time - a.time);

    // Compute weighted tangents
    float m1 = a.out_tangent * a.out_weight * (b.time - a.time); // Scale tangent by weight and duration
    float m2 = b.in_tangent * b.in_weight * (b.time - a.time);

    // Hermite spline formula
    float t2 = normalized_t * normalized_t;
    float t3 = t2 * normalized_t;

    float h1 = 2 * t3 - 3 * t2 + 1;    // Basis function for P1
    float h2 = t3 - 2 * t2 + normalized_t;       // Basis function for M1
    float h3 = -2 * t3 + 3 * t2;      // Basis function for P2
    float h4 = t3 - t2;               // Basis function for M2

    // Interpolated value
    return h1 * a.value + h2 * m1 + h3 * b.value + h4 * m2;
}

static bool compare(float time, KeyFrame const& keyframe)
{
    return time < keyframe.time;
}


void AnimationUtil::sample(Transform* dst, AnimationClip const& src, float t)
{
	for (AnimationCurve const& curve : src.curves)
	{
		std::vector<KeyFrame> const& keys = curve.keyframes;
        Transform& transform = dst[curve.bone_idx];

        auto out_key_iterator = std::upper_bound(keys.begin(), keys.end(), t, compare);
		if (out_key_iterator == keys.begin() || out_key_iterator == keys.end())
			continue;
        auto in_key_iterator = out_key_iterator - 1;

        float value = interpolate(*in_key_iterator, *out_key_iterator, t);
        switch (curve.type)
        {
        case PropertyType::PositionX:
            transform.position.x = value;
            break;
        case PropertyType::PositionY:
            transform.position.y = value;
            break;
        case PropertyType::PositionZ:
            transform.position.z = value;
            break;

        case PropertyType::RotationX:
            transform.rotation.x = value;
            break;
        case PropertyType::RotationY:
            transform.rotation.y = value;
            break;
        case PropertyType::RotationZ:
            transform.rotation.z = value;
            break;
        case PropertyType::RotationW:
            transform.rotation.w = value;
            break;

        case PropertyType::ScaleX:
            transform.scale.x = value;
            break;
        case PropertyType::ScaleY:
            transform.scale.y = value;
            break;
        case PropertyType::ScaleZ:
            transform.scale.z = value;
            break;
        }
	}
}
