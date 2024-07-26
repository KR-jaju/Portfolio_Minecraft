#pragma once

#include "ShaderManager.h"
#include "RasterizerManager.h"
#include "BlendStateManager.h"
#include "SamplerManager.h"
#include "TextureManager.h"

class PipelinePartManager
{
public:
	PipelinePartManager(ComPtr<ID3D11Device> device);
	~PipelinePartManager();

private:
	ComPtr<ID3D11Device> device;
	shared_ptr<ShaderManager> shader_manager;
	shared_ptr<RasterizerManager> rasterizer_manager;
	shared_ptr<BlendStateManager> blend_state_manager;
	shared_ptr<SamplerManager> sampler_manager;
	shared_ptr<TextureManager> texture_manager;
};

