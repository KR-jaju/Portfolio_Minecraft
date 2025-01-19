#include "pch.h"
#include "AnimationClip.h"

void	AnimationClip::sample(Armature& armature, float t, float weight, bool additive) const
{
    if (additive)
    {
        for (AnimationCurve const& curve : this->curves)
        {
            Transform& transform = armature.bone_transform.at(curve.bone_idx);
            float value = curve.evaluate(t);

            switch (curve.type)
            {
            case AnimationCurve::PropertyType::PositionX:
                transform.position.x += value * weight;
                break;
            case AnimationCurve::PropertyType::PositionY:
                transform.position.y += value * weight;
                break;
            case AnimationCurve::PropertyType::PositionZ:
                transform.position.z += value * weight;
                break;

            case AnimationCurve::PropertyType::RotationX:
                transform.rotation.x += value * weight;
                break;
            case AnimationCurve::PropertyType::RotationY:
                transform.rotation.y += value * weight;
                break;
            case AnimationCurve::PropertyType::RotationZ:
                transform.rotation.z += value * weight;
                break;
            case AnimationCurve::PropertyType::RotationW:
                transform.rotation.w += value * weight;
                break;

            case AnimationCurve::PropertyType::ScaleX:
                transform.scale.x += value * weight;
                break;
            case AnimationCurve::PropertyType::ScaleY:
                transform.scale.y += value * weight;
                break;
            case AnimationCurve::PropertyType::ScaleZ:
                transform.scale.z += value * weight;
                break;
            }
        }
    }
    else
    {
        for (AnimationCurve const& curve : this->curves)
        {
            Transform& transform = armature.bone_transform.at(curve.bone_idx);
            float value = curve.evaluate(t);

            switch (curve.type)
            {
            case AnimationCurve::PropertyType::PositionX:
                transform.position.x = value * weight;
                break;
            case AnimationCurve::PropertyType::PositionY:
                transform.position.y = value * weight;
                break;
            case AnimationCurve::PropertyType::PositionZ:
                transform.position.z = value * weight;
                break;

            case AnimationCurve::PropertyType::RotationX:
                transform.rotation.x = value * weight;
                break;
            case AnimationCurve::PropertyType::RotationY:
                transform.rotation.y = value * weight;
                break;
            case AnimationCurve::PropertyType::RotationZ:
                transform.rotation.z = value * weight;
                break;
            case AnimationCurve::PropertyType::RotationW:
                transform.rotation.w = value * weight;
                break;

            case AnimationCurve::PropertyType::ScaleX:
                transform.scale.x = value * weight;
                break;
            case AnimationCurve::PropertyType::ScaleY:
                transform.scale.y = value * weight;
                break;
            case AnimationCurve::PropertyType::ScaleZ:
                transform.scale.z = value * weight;
                break;
            }
        }
    }
}
