#include "pch.h"
#include "VisibilityProcessor.h"

VisibilityProcessor::VisibilityProcessor(RenderGroup const& input)
    : render_group(input) {}

VisibilityProcessor::VisibilityProcessor(RenderGroup&& input)
    : render_group(std::move(input)) {}

VisibilityProcessor& VisibilityProcessor::applyFrustumCulling(Frustum const& frustum)
{
    RenderGroup result;

    for (RenderGroup::Subchunk const& subchunk : this->render_group.getSubchunks())
    {
        vec3 const center = vec3(subchunk.x * 16 + 8, subchunk.y * 16 + 8, subchunk.z * 16 + 8);
        float const radius = 14.0f;

        if (!frustum.intersectsSphere(center, radius))
            continue;
        result.add(subchunk);
    }

    this->render_group = std::move(result);
    return (*this);
}

VisibilityProcessor& VisibilityProcessor::applyOcclusionCulling(const float* hiZBuffer, int width, int height)
{
    RenderGroup result;

    //TODO : Ray marching, hi-z occlusion culling
    this->render_group = std::move(result); // 가시성 필터링된 결과로 업데이트
    return (*this);
}

RenderGroup const& VisibilityProcessor::getVisibleObjects() const
{
    return this->render_group;
}
