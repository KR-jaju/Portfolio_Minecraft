#pragma once

#include "RenderPass.h"
#include "Renderer.h"

class RenderPipeline
{
public:
    RenderPipeline(TextureRegistry& texture_registry, Graphics& graphics, RenderingContext& context);

    template <typename T>
    void    addPass()
    {
        this->pass_list.emplace_back(make_unique<T>(this->renderer, this->context));
        this->output = this->context.ping ? context.textures["ldr_temporary[0]"] : context.textures["ldr_temporary[1]"]; // 최종 출력, 스왑체인으로 옮겨야함.
        this->context.ping = !this->context.ping;
    }
    void render();
private:
    std::vector<std::unique_ptr<RenderPass>> pass_list;
    ComPtr<ID3D11Texture2D> output;
    Renderer renderer;
    RenderingContext& context;

    void    blit();
};
