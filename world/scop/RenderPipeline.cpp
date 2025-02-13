#include "pch.h"
#include "RenderPipeline.h"
#include "ChunkRegistry.h"
#include "Player.h"

RenderPipeline::RenderPipeline(RenderingContext& context, AssetManager& asset_manager)
    : asset_manager(asset_manager),
    context(context)
    //renderer(context)
{

}

void    RenderPipeline::addPass(std::unique_ptr<RenderPass> pass)
{
    pass->initialize(this->context, this->asset_manager); // TODO: 순서를 명확하게
    this->pass_list.emplace_back(std::move(pass));
    this->output = this->context.ping ? context.textures["ldr_temporary[0]"] : context.textures["ldr_temporary[1]"]; // 최종 출력, 스왑체인으로 옮겨야함.
}

void RenderPipeline::render(RenderGroup const& render_group)
{
    //TODO : render_group DI하기
    for (std::unique_ptr<RenderPass>& pass : this->pass_list)
    {
        pass->execute(this->context, render_group);
    }
    this->blit();
}

void    RenderPipeline::blit()
{
    ComPtr<ID3D11DeviceContext> dc = this->context.graphics.getContext();
    ComPtr<ID3D11Texture2D> swap_chain_texture = this->context.graphics.getSwapChainTexture();

    dc->CopyResource(swap_chain_texture.Get(), this->output.Get());
}
