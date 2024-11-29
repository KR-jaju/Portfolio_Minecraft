#include "pch.h"
#include <map>
#include "AnimationResource.h"

const std::map<std::string, AnimationClip> pig_animation = {
	{
		"Scene",
		AnimationClip {{
			AnimationCurve {
				PropertyType::RotationX,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.7071, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.7071, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationY,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationZ,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationW,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.7071, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.7071, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationX,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.1667, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2083, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2917, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3750, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationY,
				1,
				{
					KeyFrame {0.0035, 0.3333, 0.0035, 0.3333, 0.0000, -0.0698, WeightedMode::Both},
					KeyFrame {0.1698, 0.3333, 0.1698, 0.3333, 0.1667, -0.0606, WeightedMode::Both},
					KeyFrame {0.4488, 0.3333, 0.4488, 0.3333, 0.2083, -0.0518, WeightedMode::Both},
					KeyFrame {0.0606, 0.3333, 0.0606, 0.3333, 0.2917, 0.0000, WeightedMode::Both},
					KeyFrame {-0.6202, 0.3333, -0.6202, 0.3333, 0.3750, -0.0518, WeightedMode::Both},
					KeyFrame {-0.4331, 0.3333, -0.4331, 0.3333, 0.4167, -0.0698, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationZ,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.1667, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2083, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2917, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3750, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationW,
				1,
				{
					KeyFrame {0.0002, 0.3333, 0.0002, 0.3333, 0.0000, 0.9976, WeightedMode::Both},
					KeyFrame {0.0100, 0.3333, 0.0100, 0.3333, 0.1667, 0.9982, WeightedMode::Both},
					KeyFrame {0.0188, 0.3333, 0.0188, 0.3333, 0.2083, 0.9987, WeightedMode::Both},
					KeyFrame {0.0013, 0.3333, 0.0013, 0.3333, 0.2917, 1.0000, WeightedMode::Both},
					KeyFrame {-0.0273, 0.3333, -0.0273, 0.3333, 0.3750, 0.9987, WeightedMode::Both},
					KeyFrame {-0.0264, 0.3333, -0.0264, 0.3333, 0.4167, 0.9976, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationX,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationY,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationZ,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationW,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationX,
				3,
				{
					KeyFrame {0.0115, 0.3333, 0.0115, 0.3333, 0.0000, 0.9976, WeightedMode::Both},
					KeyFrame {0.0120, 0.3333, 0.0120, 0.3333, 0.1250, 0.9997, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2500, 1.0000, WeightedMode::Both},
					KeyFrame {-0.0161, 0.3333, -0.0161, 0.3333, 0.3333, 0.9998, WeightedMode::Both},
					KeyFrame {-0.0273, 0.3333, -0.0273, 0.3333, 0.3750, 0.9987, WeightedMode::Both},
					KeyFrame {-0.0264, 0.3333, -0.0264, 0.3333, 0.4167, 0.9976, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationY,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.1250, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2500, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3333, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3750, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationZ,
				3,
				{
					KeyFrame {0.1736, 0.3333, 0.1736, 0.3333, 0.0000, -0.0698, WeightedMode::Both},
					KeyFrame {0.4564, 0.3333, 0.4564, 0.3333, 0.1250, -0.0246, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2500, 0.0000, WeightedMode::Both},
					KeyFrame {-0.6214, 0.3333, -0.6214, 0.3333, 0.3333, -0.0181, WeightedMode::Both},
					KeyFrame {-0.6202, 0.3333, -0.6202, 0.3333, 0.3750, -0.0518, WeightedMode::Both},
					KeyFrame {-0.4331, 0.3333, -0.4331, 0.3333, 0.4167, -0.0698, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationW,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.1250, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2500, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3333, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3750, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationX,
				4,
				{
					KeyFrame {0.8456, 0.3333, 0.8456, 0.3333, 0.0000, 0.7107, WeightedMode::Both},
					KeyFrame {1.1641, 0.3333, 1.1641, 0.3333, 0.0417, 0.7459, WeightedMode::Both},
					KeyFrame {0.7413, 0.3333, 0.7413, 0.3333, 0.0833, 0.8077, WeightedMode::Both},
					KeyFrame {-0.7413, 0.3333, -0.7413, 0.3333, 0.1250, 0.8077, WeightedMode::Both},
					KeyFrame {-1.1641, 0.3333, -1.1641, 0.3333, 0.1667, 0.7459, WeightedMode::Both},
					KeyFrame {1.4250, 0.3333, 1.4250, 0.3333, 0.2083, 0.7107, WeightedMode::Both},
					KeyFrame {3.4720, 0.3333, 3.4720, 0.3333, 0.2500, 0.8647, WeightedMode::Both},
					KeyFrame {0.6703, 0.3333, 0.6703, 0.3333, 0.2917, 1.0000, WeightedMode::Both},
					KeyFrame {-2.6659, 0.3333, -2.6659, 0.3333, 0.3333, 0.9205, WeightedMode::Both},
					KeyFrame {-2.5181, 0.3333, -2.5181, 0.3333, 0.3750, 0.7778, WeightedMode::Both},
					KeyFrame {-1.6121, 0.3333, -1.6121, 0.3333, 0.4167, 0.7107, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationY,
				4,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0417, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0833, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.1250, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.1667, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2083, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2500, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.2917, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3333, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.3750, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationZ,
				4,
				{
					KeyFrame {2.0145, 0.3333, 2.0145, 0.3333, 0.0000, -0.4975, WeightedMode::Both},
					KeyFrame {3.9614, 0.3333, 3.9614, 0.3333, 0.0417, -0.4135, WeightedMode::Both},
					KeyFrame {6.9706, 0.3333, 6.9706, 0.3333, 0.0833, -0.1674, WeightedMode::Both},
					KeyFrame {6.9706, 0.3333, 6.9706, 0.3333, 0.1250, 0.1674, WeightedMode::Both},
					KeyFrame {3.9614, 0.3333, 3.9614, 0.3333, 0.1667, 0.4135, WeightedMode::Both},
					KeyFrame {-0.1505, 0.3333, -0.1505, 0.3333, 0.2083, 0.4975, WeightedMode::Both},
					KeyFrame {-5.9696, 0.3333, -5.9696, 0.3333, 0.2500, 0.4010, WeightedMode::Both},
					KeyFrame {-9.0502, 0.3333, -9.0502, 0.3333, 0.2917, 0.0000, WeightedMode::Both},
					KeyFrame {-5.7837, 0.3333, -5.7837, 0.3333, 0.3333, -0.3532, WeightedMode::Both},
					KeyFrame {-1.7312, 0.3333, -1.7312, 0.3333, 0.3750, -0.4820, WeightedMode::Both},
					KeyFrame {-0.3719, 0.3333, -0.3719, 0.3333, 0.4167, -0.4975, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationW,
				4,
				{
					KeyFrame {-0.5919, 0.3333, -0.5919, 0.3333, 0.0000, -0.4975, WeightedMode::Both},
					KeyFrame {-0.8149, 0.3333, -0.8149, 0.3333, 0.0417, -0.5221, WeightedMode::Both},
					KeyFrame {-0.5189, 0.3333, -0.5189, 0.3333, 0.0833, -0.5654, WeightedMode::Both},
					KeyFrame {0.5189, 0.3333, 0.5189, 0.3333, 0.1250, -0.5654, WeightedMode::Both},
					KeyFrame {0.8149, 0.3333, 0.8149, 0.3333, 0.1667, -0.5221, WeightedMode::Both},
					KeyFrame {2.6340, 0.3333, 2.6340, 0.3333, 0.2083, -0.4975, WeightedMode::Both},
					KeyFrame {5.9696, 0.3333, 5.9696, 0.3333, 0.2500, -0.3026, WeightedMode::Both},
					KeyFrame {1.6269, 0.3333, 1.6269, 0.3333, 0.2917, 0.0000, WeightedMode::Both},
					KeyFrame {-4.8399, 0.3333, -4.8399, 0.3333, 0.3333, -0.1671, WeightedMode::Both},
					KeyFrame {-3.9650, 0.3333, -3.9650, 0.3333, 0.3750, -0.4033, WeightedMode::Both},
					KeyFrame {-2.2595, 0.3333, -2.2595, 0.3333, 0.4167, -0.4975, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationX,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationY,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationZ,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationW,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationX,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationY,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationZ,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::RotationW,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionX,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionY,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionZ,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionX,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionY,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.7500, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.7500, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionZ,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionX,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionY,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.7500, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.7500, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionZ,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionX,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.3750, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.3750, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionY,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.6250, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.6250, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionZ,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionX,
				4,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, -0.3750, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, -0.3750, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionY,
				4,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.6250, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.6250, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionZ,
				4,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionX,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.1250, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.1250, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionY,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.7500, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.7500, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionZ,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionX,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, -0.1250, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, -0.1250, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionY,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.7500, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.7500, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::PositionZ,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 0.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 0.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleX,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleY,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleZ,
				0,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleX,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleY,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleZ,
				1,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleX,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleY,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleZ,
				2,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleX,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleY,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleZ,
				3,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleX,
				4,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleY,
				4,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleZ,
				4,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleX,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleY,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleZ,
				5,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleX,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleY,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
			AnimationCurve {
				PropertyType::ScaleZ,
				6,
				{
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.0000, 1.0000, WeightedMode::Both},
					KeyFrame {0.0000, 0.3333, 0.0000, 0.3333, 0.4167, 1.0000, WeightedMode::Both},
				}
			},
		}}
	},
};
