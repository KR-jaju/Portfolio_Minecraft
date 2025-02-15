#include "pch.h"
#include "VisibilityProcessor.h"

//VisibilityProcessor::VisibilityProcessor(RenderGroup const& input)
//    : render_group(input) {}
//
//VisibilityProcessor::VisibilityProcessor(RenderGroup&& input)
//    : render_group(std::move(input)) {}






void VisibilityProcessor::process(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& render_group, Frustum const& frustum, RenderGroup const& occluders)
{
    this->render_group = render_group; // 복사
    this->applyFrustumCulling(frustum);
}


//
//VisibilityProcessor& VisibilityProcessor::applyOcclusionCulling(RenderGroup const& occluder_group)
//{
//    RenderGroup result;
//
//
//
//
//
//    this->render_group = std::move(result); // 가시성 필터링된 결과로 업데이트
//    return (*this);
//}

RenderGroup const& VisibilityProcessor::getResult() const
{
    return this->render_group;
}



void VisibilityProcessor::applyFrustumCulling(Frustum const& frustum)
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
}

