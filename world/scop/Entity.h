#pragma once

#include <vector>
#include "Texture.h"
#include "Buffer.h"
#include "EntityVertex.h"

class EntityGeometry;

class Entity
{
public:
	Entity(shared_ptr<EntityGeometry> geometry, shared_ptr<Texture> texture);
	void	render(ComPtr<ID3D11DeviceContext> context) const;
	BoneData& getBoneTransforms();
	BoneData const& getBoneTransforms() const;
private:
	vec3 position;
	vec3 rotation;
	vec3 scale;
	vec3 aabb_min;
	vec3 aabb_max;
	shared_ptr<Texture> texture;
	shared_ptr<EntityGeometry> geometry;
	BoneData bone_transform;
};

//-----------------테스트용----------------------