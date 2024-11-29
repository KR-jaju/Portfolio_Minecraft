#pragma once

#include <vector>
#include "Texture.h"
#include "Buffer.h"
#include "EntityVertex.h"

class EntityGeometry;

class Entity
{
public:

	virtual ~Entity() = default;
	virtual void	update() = 0;
	virtual void	render(ComPtr<ID3D11DeviceContext> context) const = 0;
	virtual void	updateBoneMatrix() = 0;
	virtual BoneData const& getBoneTransform() const = 0;
};