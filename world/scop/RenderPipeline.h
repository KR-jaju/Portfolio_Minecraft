#pragma once

#include "RenderPass.h"
#include "RenderGroup.h"
#include "Renderer.h"

class RenderPipeline
{
public:
    RenderPipeline(RenderingContext& context, AssetManager& asset_manager);

    void addPass(std::unique_ptr<RenderPass> pass);
    void render(RenderGroup const& render_group);
private:
    AssetManager& asset_manager;
    std::vector<std::unique_ptr<RenderPass>> pass_list;
    ComPtr<ID3D11Texture2D> output;
    //Renderer renderer;
    RenderingContext& context;

    void    blit();
};
