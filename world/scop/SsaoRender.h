#pragma once

#include "WorldUtils.h"

class MapUtils;
class DeferredGraphics;
class DeferredBuffer;
class RasterizerState;
class VertexShader;
class PixelShader;
class InputLayout;
class DepthMap;
class SamplerState;
template <typename T> class Buffer;
class ConstantBuffer;

class SsaoRender
{
public:
	SsaoRender(DeferredGraphics* d_graphic, 
		UINT width, UINT height);
	void render(Mat const& cam_view, Mat const& cam_proj);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	shared_ptr<DeferredBuffer> getDBuffer();

private:
	void setPipe();
	void buildOffset();
	void buildRandomVecTexture();

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;
	shared_ptr<ConstantBuffer> cbuffer;

private:
	DeferredGraphics* d_graphic;
	vec4 mOffsets[14];
	ComPtr<ID3D11ShaderResourceView> random_vec_SRV;
	D3D11_VIEWPORT view_port = { 0, };
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
};

