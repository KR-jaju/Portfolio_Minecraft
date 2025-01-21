#pragma once

#include <string>
#include "AnimationClip.h"
#include "AnimationStateInfo.h"

class AnimationLayer
{
public:
	AnimationLayer();
	void	update(Armature& armature, float dt);
	bool	isStable() const;
	float	getTime() const;
	void	setAnimation(std::shared_ptr<AnimationClip> const& animation, AnimationStateInfo info, float transition_duration = 0.0f, float transition_offset = 0.0f);

	virtual void	updateState() = 0; // update animation state machine
private:
	std::shared_ptr<AnimationClip> src_animation;
	std::shared_ptr<AnimationClip> dst_animation;
	AnimationStateInfo src_state;
	AnimationStateInfo dst_state;
	float	src_time; // sample time of source animation
	float	dst_time;
	float	transition_weight;
	float	transition_duration;
	bool	is_stable; // not transitioning

	bool	stableState(Armature& armature, float dt);
	bool	transitioningState(Armature& armature, float dt);
};
