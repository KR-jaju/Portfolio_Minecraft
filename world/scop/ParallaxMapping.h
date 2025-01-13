#pragma once

#include "WorldUtils.h"

enum class RTVIndex
{
	color,
	w_position,
	w_normal,
	rma, //r: roughness, g: metallic, b: ambient occlusion
	ssao_normal,
};

class DeferredGraphics;
class DeferredBuffer;
class TextureArray;
class SamplerState;
class VertexShader;
class PixelShader;
class InputLayout;
class RasterizerState;
class ConstantBuffer;
template<typename T> class Buffer;

class ParallaxMapping
{
public:
	ParallaxMapping(DeferredGraphics* graphic, UINT width, UINT height);
	ParallaxMapping(ParallaxMapping const&) = delete;
	ParallaxMapping& operator=(ParallaxMapping const&) = delete;
	~ParallaxMapping();
	void setRTV();
	void render(vec3 const& cam_pos);
	ComPtr<ID3D11ShaderResourceView> getSRV(RTVIndex idx);
	ComPtr<ID3D11RenderTargetView> getRTV(RTVIndex idx);

private:
	void setPipe();
	
private:
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<ConstantBuffer> eye_poe_cbuffer;
};

