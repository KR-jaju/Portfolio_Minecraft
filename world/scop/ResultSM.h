#pragma once

#include "WorldUtils.h"

class SunMoon;
class Blur;
class DeferredGraphics;
class DeferredBuffer;
class VertexShader;
class PixelShader;
class InputLayout;
class SamplerState;
class RasterizerState;
template<typename T> class Buffer;

class ResultSM
{
public:
	ResultSM(DeferredGraphics* graphic, UINT width, UINT height);
	void render(
		vec3 const& cam_pos,
		Mat const& cam_view,
		Mat const& cam_proj
	);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	vec3 getDirectionalLightPos();

private:
	void setPipe();

private:
	shared_ptr<SunMoon> sun_moon;
	shared_ptr<Blur> blur;
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_sahder;
	shared_ptr<RasterizerState> rasterizer_sate;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
};

