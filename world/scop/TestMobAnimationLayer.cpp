#include "pch.h"
#include "TestMobAnimationLayer.h"

TestMobAnimationLayer::TestMobAnimationLayer(AssetManager& asset_manager, TestMobAnimatorContext& animator_context)
	: fsm({ {State::Idle, &TestMobAnimationLayer::idleState} }, State::Idle),
	animator_context(animator_context),
	idle_animation(asset_manager.load<AnimationClip>(L"idle.json"))
{

}

void	TestMobAnimationLayer::updateState()
{
	this->fsm.update(*this);
}

bool	TestMobAnimationLayer::idleState()
{
	if (this->getTime() > this->idle_animation->getLength())
		this->setAnimation(this->idle_animation, TestMobAnimationLayer::idle_state_info);
	return (false);
}