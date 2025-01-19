#pragma once

#include <string>
#include "AnimationState.h"

class AnimationLayer
{
public:
	AnimationLayer();
	void	update(Armature& armature, float dt);
	bool	isStable() const;
	float	getTime() const;
	void	setTransition(AnimationState const* dst_state, float duration, float offset);
	virtual void	updateState() = 0; // update animation state machine
private:
	AnimationState const* src_state;
	AnimationState const* dst_state;
	float	src_time; // sample time of source animation
	float	dst_time;
	float	transition_weight;
	float	transition_duration;
	bool	is_stable; // not transitioning

	bool	stableState(Armature& armature, float dt);
	bool	transitioningState(Armature& armature, float dt);
};
