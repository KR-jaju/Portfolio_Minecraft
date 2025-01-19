#include "pch.h"
#include "TestMobAnimator.h"

void	TestMobAnimator::update(Armature& armature, float dt)
{
	this->base_layer.update(armature, dt);
}
