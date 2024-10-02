#pragma once

#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "TextureResource.h"
#include "SamplerState.h"

struct PipelineInfo
{
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<BlendState> blend_state;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> device_context);
	~Pipeline();
	
	void UpdatePipeline(PipelineInfo info);
	void SetVertexBuffer(shared_ptr<Buffer<VertexUV>> buffer);
	void SetColorVertexBuffer(shared_ptr<Buffer<Vertex>> buffer);
	void SetIndexBuffer(shared_ptr<Buffer<uint32>> buffer);

	void SetVSConstantBuffer(uint32 slot, shared_ptr<ConstantBuffer> buffer);
	void SetPSConstantBuffer(uint32 slot, shared_ptr<ConstantBuffer> buffer);

	void SetVSTexture(uint32 slot, shared_ptr<TextureResource> texture);
	void SetPSTexture(uint32 slot, shared_ptr<TextureResource> texture);

	void SetVSSamplerState(uint32 slot, shared_ptr<SamplerState> sampler_state);
	void SetPSSamplerState(uint32 slot, shared_ptr<SamplerState> sampler_state);

	void Draw(uint32 vertex_count, uint32 start_vertex_location);
	void DrawIndexed(uint32 index_count, uint32 start_vertex_location, uint32 base_vertex_location);

private:
	ComPtr<ID3D11DeviceContext> device_context;

};

