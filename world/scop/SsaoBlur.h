#pragma once

#include "WorldUtils.h"

class DeferredGraphics;
template <typename T> class Buffer;
class RasterizerState;
class SamplerState;
class VertexShader;
class PixelShader;
class InputLayout;
class DeferredBuffer;
class ConstantBuffer;

class SsaoBlur
{
public:
	SsaoBlur(DeferredGraphics* d_graphic, UINT width, UINT height);
	void render(
		int wh_flag, 
		Mat const& proj,
		Mat const& view,
		float num
	);
	ComPtr<ID3D11ShaderResourceView> getWidthSRV();
	ComPtr<ID3D11ShaderResourceView> getHeightSRV();
	shared_ptr<DeferredBuffer> const& getWidthDBuffer();
	shared_ptr<DeferredBuffer> const& getHeightDBuffer();

private:
	void setPipe();

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;
	shared_ptr<ConstantBuffer> cbuffer;

private:
	DeferredGraphics* d_graphic;
	UINT width;
	UINT height;
	D3D11_VIEWPORT view_port = { 0, };
	shared_ptr<DeferredBuffer> d_buffer_width;
	shared_ptr<DeferredBuffer> d_buffer_height;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
};

