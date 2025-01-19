#pragma once

#include "Entity.h"
#include "TestMobAnimator.h"

class TestMob : public Entity
{
public:
	TestMob();
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


	bool	isVisible() const;
	SkinnedMesh* getMesh() const;
private:
	Armature armature;
	vec3	position;
	vec3	rotation;
	vec3	velocity;
	int		contact_faces;

	TestMobAnimator animator;
};