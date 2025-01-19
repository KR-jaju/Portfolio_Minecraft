#include "pch.h"
#include "AnimationCurve.h"
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

float	AnimationCurve::evaluate(float t) const
{
	if (!this->hasValue(t))
		return (0.0f);
    std::vector<KeyFrame> const& keys = this->keyframes;
    auto out_key_iterator = std::upper_bound(keys.begin(), keys.end(), t, compare);
    auto in_key_iterator = out_key_iterator - 1;
    float value = interpolate(*in_key_iterator, *out_key_iterator, t);

    return (value);
}

bool	AnimationCurve::hasValue(float t) const
{
    if (this->keyframes.size() == 0)
        return (false);
	return (this->keyframes.front().time <= t && t <= this->keyframes.back().time);
}