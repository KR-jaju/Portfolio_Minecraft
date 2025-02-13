#pragma once
#include "RenderingContext.h"

class RenderGroup;
class AssetManager;

class RenderPass
{
public:
	virtual ~RenderPass() = default;
	virtual void execute(RenderingContext& context, RenderGroup const& render_group) = 0;
private:
	friend class RenderPipeline;
	virtual void initialize(RenderingContext& context, AssetManager& asset_manager) = 0;
};