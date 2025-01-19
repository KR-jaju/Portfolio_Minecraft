#include "pch.h"
#include "RenderPipeline.h"
#include "ChunkRegistry.h"
#include "Player.h"

RenderPipeline::RenderPipeline(TextureRegistry& texture_registry, Graphics& graphics, RenderingContext& context)
    : context(context),
    renderer(context)
{

}

void RenderPipeline::render()
{
    for (std::unique_ptr<RenderPass>& pass : this->pass_list)
    {
        pass->execute(this->context);
    }
    this->blit();
}

void    RenderPipeline::blit()
{
    ComPtr<ID3D11DeviceContext> dc = this->context.graphics.getContext();
    ComPtr<ID3D11Texture2D> swap_chain_texture = this->context.graphics.getSwapChainTexture();

    dc->CopyResource(swap_chain_texture.Get(), this->output.Get());
}
