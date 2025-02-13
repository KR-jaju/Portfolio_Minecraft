#pragma once

#include "Entity.h"
#include "SubchunkMesh.h"
#include "RenderingContext.h"

class Renderer
{
public:
	Renderer(RenderingContext& context);

	void	render(SubchunkMesh& data, ivec2 chunk_idx);
	void	render(Entity& entity);
private:
	RenderingContext& context;
	ConstantBuffer	model_cb;
	ConstantBuffer	bindpose_cb;
	ConstantBuffer	pose_cb;
};
