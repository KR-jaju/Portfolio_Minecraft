#pragma once
#include "RenderingContext.h"

#include "Renderer.h"

class RenderPass
{
public:
	virtual ~RenderPass() = default;
	virtual void execute(RenderingContext& context) = 0;
};