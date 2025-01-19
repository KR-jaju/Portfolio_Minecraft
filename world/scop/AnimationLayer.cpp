#include "pch.h"
#include "AnimationLayer.h"
#include <algorithm>

AnimationLayer::AnimationLayer()
	: src_state(nullptr), dst_state(nullptr),
	src_time(0), dst_time(0),
	transition_weight(0), transition_duration(0),
	is_stable(true)
{}

void	AnimationLayer::update(Armature& armature, float dt)
{
	this->src_time += dt;
	this->dst_time += dt;
	this->updateState(); //애니매이션 상태 머신 업데이트
	while (true)
	{
		if (this->is_stable && !this->stableState(armature, dt))
			break;
		else if (!this->is_stable && !this->transitioningState(armature, dt))
			break;
	} // (안정) / (전이 중) 상태 머신
}

bool	AnimationLayer::isStable() const
{
	return (this->is_stable);
}

float	AnimationLayer::getTime() const
{
	return (this->dst_time);
}

void	AnimationLayer::setTransition(AnimationState const* dst_state, float duration, float offset)
{
	this->src_state = this->dst_state;
	this->dst_state = dst_state;
	if (this->src_state == nullptr) // 트랜지션할 시작 상태가 없음!
		return;
	this->src_time = this->dst_time;
	this->dst_time = offset;
	this->transition_weight = 0;
	this->transition_duration = duration;
	this->is_stable = false;
}



bool	AnimationLayer::stableState(Armature& armature, float dt)
{
	AnimationClip const* animation = this->dst_state->animation_clip;
	float	t = this->dst_time;

	animation->sample(armature, t); // TODO : State 값을 보고 판단하는게 좋음 (loop나 cycle offset)
	return (false);
}

bool	AnimationLayer::transitioningState(Armature& armature, float dt)
{
	this->transition_weight = std::fminf(this->transition_weight + dt / this->transition_duration, 1.0f); // duration 동안 0.0f -> 1.0f
	if (this->transition_weight == 1)
		return (this->is_stable = true, true); // 트랜지션 종료
	this->src_state->animation_clip->sample(armature, this->src_time, 1.0f - this->transition_weight);
	this->dst_state->animation_clip->sample(armature, this->dst_time, this->transition_weight, true);
	return (false);
}


/*
	AnimationGraph::State const* state = this->animation_state;

	for (AnimationGraph::Transition const& transition : state->transitions)
	{
		if (this->src_time > transition.exit_time && !transition.rule(parameters))
			continue;
		this->animation_state = transition.dst_state;
		this->animation_transition = &transition;
		this->dst_time = transition.offset; // 목표 애니매이션이 샘플링되기 시작하는 t
		this->transition_weight = 0; // src애니매이션이 100%인 상태로 시작
		this->is_stable = false;
		return (true); // 바로 진행
	}


		AnimationTransition const& transition = this->transition;

	AnimationGraph::State const* dst_state = this->animation_state;

	for (AnimationGraph::Transition const& transition : dst_state->transitions)
	{
		if (this->dst_time > transition.exit_time || !transition.rule(parameters))
			continue;
		this->animation_state = transition.dst_state;
		this->animation_transition = &transition;
		this->src_time = this->dst_time;
		this->dst_time = transition.offset;
		this->transition_weight = 0; // src애니매이션이 100%인 상태로 시작
		return (true); // 바로 진행
	}
*/