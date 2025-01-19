#pragma once

#include "Entity.h"
#include "RenderingContext.h"

class Renderer
{
public:
	Renderer(RenderingContext& context);
	void	render(Entity& entity);
private:
	RenderingContext& context;
	ConstantBuffer	model_cb;
	ConstantBuffer	armature_cb;
};
