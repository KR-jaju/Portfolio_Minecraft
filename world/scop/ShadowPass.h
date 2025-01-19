#pragma once

#include "RenderPass.h"

class ShadowPass : public RenderPass
{
public:
	void execute(RenderingContext& context);
private:
	void bind();
	void unbind();
};
