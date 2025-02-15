#pragma once
#include <vector>
#include "Frustum.h"
#include "RenderGroup.h"
#include "Camera.h"
#include <functional>

class VisibilityProcessor {
public:
    //std::shared_future<void> processAsync(RenderGroup const& render_group, RenderGroup const& occluders);
    void process(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& render_group, Frustum const& frustum, RenderGroup const& occluders);
    RenderGroup const& getResult() const;
private:
    RenderGroup render_group;

    void applyFrustumCulling(Frustum const& frustum);
    //VisibilityProcessor& applyOcclusionCulling(RenderGroup const& occluder_group);
};
