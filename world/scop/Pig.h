#pragma once

#include "Entity.h"
#include "FSM.h"
#include "Transform.h"

class SkinnedMesh;

class Pig : public Entity
{
public:
	Pig(shared_ptr<SkinnedMesh> geometry, shared_ptr<Texture> texture);
	void	update(float dt);
	void	render();

	vec3	getPosition() const;
	vec3	getRotation() const;
	bool	shouldDespawn() const;
	void	setPosition(vec3 position);
	ivec2	getChunkIndex() const;
	vec3	getVelocity() const;
	void	setVelocity(vec3 velocity);
	int		getContactFaces() const;
	void	setContactFaces(int faces);
	AABB const& getBoundingBox() const;
private:
	vec3	position;
	vec3	rotation;
	vec3	velocity;
	AABB	bounding_box;
	int		contact_faces;


	//shared_ptr<Texture> texture;
	//shared_ptr<SkinnedMesh> mesh;
	//std::vector<Transform> armature;
	//BoneData bone_transform;

	//FSM<Pig>	behavior_fsm;
	//Animator	animator;

	//vec3 position;
	//vec3 rotation;
	//vec3 scale;

	//bool	behaviorIdleState();

	//bool	animationIdleState();
	//bool	animationIdleToWalkState();
};
