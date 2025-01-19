#pragma once

#include <vector>
#include "Texture.h"
#include "Buffer.h"
#include "EntityVertex.h"
#include <tuple>
#include "AssetManager.h"

#include "InputRegistry.h"

#include "AABB.h"
#include "SkinnedMesh.h"

class EntityGeometry;
class EntityAssetManager;
class MobAI;
class SkinnedMesh;

class Entity
{
public:

	//Entity();
	virtual ~Entity() = default;

	//void	update(vec3 player_position);
	//void	render(ComPtr<ID3D11DeviceContext> context) const;
	//void	updateBoneMatrix();
	////BoneData const& getBoneTransform() const;
	//vec3	getPosition() const;
	//vec3	getRotation() const;

	//bool	shouldDespawn() const;

	//void	setMesh(std::shared_ptr<SkinnedMesh> mesh);
	//std::shared_ptr<SkinnedMesh>	getMesh() const;

	virtual void	update(float dt) = 0;

	virtual vec3	getPosition() const = 0;
	virtual vec3	getRotation() const = 0;
	virtual bool	shouldDespawn() const = 0;

	virtual void	setPosition(vec3 position) = 0;
	virtual ivec2 getChunkIndex() const = 0;
	virtual vec3	getVelocity() const = 0;
	virtual void	setVelocity(vec3 velocity) = 0;
	virtual int		getContactFaces() const = 0;
	virtual void	setContactFaces(int faces) = 0;
	virtual AABB const&	getBoundingBox() const = 0;

	//Rendering Options
	virtual bool	isVisible() const = 0;
	virtual SkinnedMesh* getMesh() const = 0;

private:
	//std::shared_ptr<SkinnedMesh>	mesh;
	//
	//vec3 position;
	//vec3 rotation;
	//vec3 scale;

	//std::vector<Transform> armature;

	////std::unique_ptr<MobAI> behavior;
	//std::unique_ptr<Animator>	animator;

	//typedef Mat TransformData[32];
	//TransformData bone_transform;
};