#include "pch.h"
#include "PipelinePartManager.h"

PipelinePartManager::PipelinePartManager(
	ComPtr<ID3D11Device> device
)
	: device(device)
{
	this->shader_manager = make_shared<ShaderManager>(this->device);
	this->rasterizer_manager = 
		make_shared<RasterizerManager>(this->device);
	this->blend_state_manager =
		make_shared<BlendStateManager>(this->device);
	this->sampler_manager =
		make_shared<SamplerManager>(this->device);
	this->texture_manager =
		make_shared<TextureManager>(this->device);
}

PipelinePartManager::~PipelinePartManager()
{
}
