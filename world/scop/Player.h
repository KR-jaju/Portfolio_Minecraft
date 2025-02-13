#pragma once

#include "Entity.h"
#include "Camera.h"

/*
TODO: 카메라가 여기 들어있는게 조금 이상함
*/

class Player : public Entity
{
public:
	Player(InputRegistry& input_registry, AssetManager& asset_manager);

	void	update(float dt);

	vec3	getPosition() const;
	vec3	getRotation() const;
	bool	shouldDespawn() const;

	void	setPosition(vec3 position);

	Camera const& getCamera() const;
	bool	hasInputOwnership() const;
	void	setInputOwnership(bool has_ownership);

	ivec2	getChunkIndex() const;

	vec3	getVelocity() const;
	void	setVelocity(vec3 velocity);
	int		getContactFaces() const;
	void	setContactFaces(int faces);

	AABB const& getBoundingBox() const;

	bool	isVisible() const;
	SkinnedMesh* getMesh() const;
	BoneData const& getPose() const;
private:
	InputRegistry& input_registry;

	Camera	camera;

	vec3	position;
	vec3	rotation;
	vec3	velocity;
	bool	owns_input;
	int contact_faces;
	AABB	bounding_box;
	float	time;
	float	bobbing_frequency;
	float	bobbing_amplitude;

	void	groundMovement(InputRegistry& input_registry, float dt);
	void	airMovement(InputRegistry& input_registry, float dt);
};