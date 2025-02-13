#pragma once
#include <vector>
#include "Frustum.h"
#include "RenderGroup.h"

class VisibilityProcessor {
public:
    VisibilityProcessor(RenderGroup const& input);
    VisibilityProcessor(RenderGroup&& input);
    VisibilityProcessor& applyFrustumCulling(Frustum const& frustum);
    VisibilityProcessor& applyOcclusionCulling(const float* hiZBuffer, int width, int height);
    RenderGroup const& getVisibleObjects() const;
private:
    RenderGroup render_group;
};
