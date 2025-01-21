#include "pch.h"
#include "AnimationLayer.h"
#include <algorithm>

AnimationLayer::AnimationLayer()
	: src_animation(nullptr), dst_animation(nullptr),
	src_state(), dst_state(),
	src_time(0), dst_time(0),
	transition_weight(0), transition_duration(0),
	is_stable(true)
{}

void	AnimationLayer::update(Armature& armature, float dt)
{
	this->src_time += dt * 0.1f;
	this->dst_time += dt * 0.1f;
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

void	AnimationLayer::setAnimation(std::shared_ptr<AnimationClip> const& animation, AnimationStateInfo info, float transition_duration, float transition_offset)
{
	this->src_animation = this->dst_animation;
	this->dst_animation = animation;
	if (this->src_animation == nullptr) // 트랜지션할 시작 상태가 없음!
		this->src_animation = animation;
	this->src_state = this->dst_state;
	this->src_state = info;
	this->src_time = this->dst_time;
	this->dst_time = transition_offset;
	this->transition_weight = 0;
	this->transition_duration = transition_duration;
	this->is_stable = false;
}



bool	AnimationLayer::stableState(Armature& armature, float dt)
{
	float	t = this->dst_time;

	if (this->dst_animation == nullptr)
		return (false);
	this->dst_animation->sample(armature, t); // TODO : State 값을 보고 판단하는게 좋음 (loop나 cycle offset)
	return (false);
}

bool	AnimationLayer::transitioningState(Armature& armature, float dt)
{
	this->transition_weight = std::fminf(this->transition_weight + dt / this->transition_duration, 1.0f); // duration 동안 0.0f -> 1.0f
	if (this->transition_weight == 1)
		return (this->is_stable = true, true); // 트랜지션 종료
	this->src_animation->sample(armature, this->src_time, 1.0f - this->transition_weight);
	this->dst_animation->sample(armature, this->dst_time, this->transition_weight, true);
	return (false);
}
