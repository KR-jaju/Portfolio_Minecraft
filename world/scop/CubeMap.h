#pragma once

#include "WorldUtils.h"

class DeferredGraphics;
class VertexShader;
class SamplerState;
class PixelShader;
class InputLayout;
class RasterizerState;
class DeferredBuffer;
template<typename T> class Buffer;

class CubeMap
{
public:
	CubeMap(DeferredGraphics* dgraphic,
		UINT width, UINT height);
	void render(Mat const& cam_view, Mat const& cam_proj,
		vec3 const& cam_pos);
	ComPtr<ID3D11ShaderResourceView> getSRV();
private:
	void setPipe();
	void makeCubeSRV();
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	DeferredGraphics* d_graphic;
	UINT width;
	UINT height;

private:
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<RasterizerState> rasterizer_state;
	ComPtr<ID3D11ShaderResourceView> cube_srv;
};

