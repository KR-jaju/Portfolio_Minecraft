#pragma once

#include "Entity.h"
#include "FSM.h"
#include "Transform.h"

class SkinnedMesh;

class Pig : public Entity
{
public:
	Pig(shared_ptr<SkinnedMesh> geometry, shared_ptr<Texture> texture);
	void	update();
	void	render(ComPtr<ID3D11DeviceContext> context) const;
	void	updateBoneMatrix();
	BoneData const& getBoneTransform() const;
private:
	FSM<Pig>	behavior_fsm;
	FSM<Pig>	animation_fsm;

	float animation_time;
	float animation_speed;

	shared_ptr<Texture> texture;
	shared_ptr<SkinnedMesh> mesh;
	std::vector<Transform> armature;
	BoneData bone_transform;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	bool	behaviorIdleState();

	bool	animationIdleState();
};
