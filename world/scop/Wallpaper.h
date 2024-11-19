#pragma once

#include "Block.h"

class CubeMap;
class ResultSM;
class DeferredGraphics;
class DeferredBuffer;
class VertexShader;
class RasterizerState;
class PixelShader;
class InputLayout;
class SamplerState;
template<typename T> class Buffer;

class Wallpaper
{
public:
	Wallpaper(DeferredGraphics* graphic,
		UINT width, UINT height);
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
	shared_ptr<CubeMap> cube_map;
	shared_ptr<ResultSM> sun_moon;
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	DeferredGraphics* d_graphic;
	UINT width;
	UINT height;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> verex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
};

