#pragma once

#include "TestMobAnimationLayer.h"

class TestMobAnimator
{
public:
	void	update(Armature& armature, float dt);
private:
	TestMobAnimationLayer base_layer;
};

