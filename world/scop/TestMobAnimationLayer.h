#pragma once

#include "AnimationLayer.h"
#include "FSM.h"
#include "AssetManager.h"
#include "AnimationStateInfo.h"

#include "TestMobAnimatorContext.h"

class TestMobAnimationLayer : public AnimationLayer
{
public:
	TestMobAnimationLayer(AssetManager& asset_manager, TestMobAnimatorContext& animator_context);
	void	updateState(); // 애니매이션 상태 머신
private:
	enum class State
	{
		Idle
	};

	TestMobAnimatorContext& animator_context;
	std::shared_ptr<AnimationClip> idle_animation;
	FSM<TestMobAnimationLayer, State> fsm;

	static constexpr AnimationStateInfo idle_state_info = { 1.0f, false };
	bool	idleState();
};