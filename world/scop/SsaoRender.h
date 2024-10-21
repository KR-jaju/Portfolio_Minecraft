#pragma once

#include "WorldUtils.h"
#include "BlendState.h"
#include "InputLayouts.h"

class MapUtils;
class DeferredGraphics;
class DeferredBuffer;
class RasterizerState;
class VertexShader;
class PixelShader;
class InputLayout;
class BlendState;
class DepthMap;
class SamplerState;
template <typename T> class Buffer;

class SsaoRender
{
public:
	SsaoRender(DeferredGraphics* d_graphic, 
		UINT width, UINT height);
	void render(Mat const& cam_proj);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	shared_ptr<DeferredBuffer> getDBuffer();

private:
	void setPipe();
	void buildOffset();
	void buildRandomVecTexture();

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	DeferredGraphics* d_graphic;
	vec4 mOffsets[14];
	ComPtr<ID3D11ShaderResourceView> random_vec_SRV;
	D3D11_VIEWPORT view_port = { 0, };
	InputLayouts layout;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<BlendState> blend_state;
};

