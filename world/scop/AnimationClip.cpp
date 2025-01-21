#include "pch.h"
#include "AnimationClip.h"

static KeyFrame::WeightedMode weightedModeFrom(std::string str)
{
    if (str == "In")
        return (KeyFrame::WeightedMode::In);
    else if (str == "Out")
        return (KeyFrame::WeightedMode::Out);
    else if (str == "Both")
        return (KeyFrame::WeightedMode::Both);
    else if (str == "None")
        return (KeyFrame::WeightedMode::None);
    throw std::runtime_error("Invalid property type");
}

static AnimationCurve::PropertyType propertyTypeFrom(std::string str)
{
    if (str == "PropertyType::PositionX")
        return (AnimationCurve::PropertyType::PositionX);
    else if (str == "PropertyType::PositionY")
        return (AnimationCurve::PropertyType::PositionY);
    else if (str == "PropertyType::PositionZ")
        return (AnimationCurve::PropertyType::PositionZ);
    else if (str == "PropertyType::RotationX")
        return (AnimationCurve::PropertyType::RotationX);
    else if (str == "PropertyType::RotationY")
        return (AnimationCurve::PropertyType::RotationY);
    else if (str == "PropertyType::RotationZ")
        return (AnimationCurve::PropertyType::RotationZ);
    else if (str == "PropertyType::RotationW")
        return (AnimationCurve::PropertyType::RotationW);
    else if (str == "PropertyType::ScaleX")
        return (AnimationCurve::PropertyType::ScaleX);
    else if (str == "PropertyType::ScaleY")
        return (AnimationCurve::PropertyType::ScaleY);
    else if (str == "PropertyType::ScaleZ")
        return (AnimationCurve::PropertyType::ScaleZ);
    throw std::runtime_error("Invalid property type");
}

AnimationClip::AnimationClip(std::wstring const& path)
{
    std::ifstream ifs(path);
    nlohmann::json json;

    ifs >> json;
    for (auto const& curve : json["curves"])
    {
        AnimationCurve animation_curve = {};

        animation_curve.bone_idx = curve["bone_idx"].get<int>();
        animation_curve.type = propertyTypeFrom(curve["property_name"].get<std::string>());
        for (auto const& keyframe : curve["keyframes"])
        {
            KeyFrame animation_keyframe = {};

            animation_keyframe.in_tangent = keyframe["in_tangent"].get<float>();
            animation_keyframe.in_weight = keyframe["in_weight"].get<float>();
            animation_keyframe.out_tangent = keyframe["out_tangent"].get<float>();
            animation_keyframe.out_weight = keyframe["out_weight"].get<float>();
            animation_keyframe.time = keyframe["time"].get<float>();
            animation_keyframe.value = keyframe["value"].get<float>();
            animation_keyframe.weighted_mode = weightedModeFrom(keyframe["weighted_mode"].get<std::string>());
            animation_curve.keyframes.push_back(animation_keyframe);
        }
        this->curves.push_back(animation_curve);
    }
    this->length = json["length"].get<float>();
}

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

          /*  case AnimationCurve::PropertyType::RotationX:
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
                break;*/
                //TODO
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
                transform.position.x = -value * weight;
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
                transform.rotation.y = -value * weight;
                break;
            case AnimationCurve::PropertyType::RotationZ:
                transform.rotation.z = -value * weight;
                break;
            case AnimationCurve::PropertyType::RotationW:
                transform.rotation.w = value * weight;
                break;
                //TODO
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

float   AnimationClip::getLength() const
{
    return (this->length);
}
