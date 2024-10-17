#pragma once

#include "ShadowRender.h"
#include "GeoRender.h"
#include "WorldUtils.h"
#include "BlendState.h"
#include "Ssao.h"

class DeferredGraphics;
class MapUtils;
template <typename T> class Buffer;
class Graphics;
class Texture;

class DeferredRendering
{
public:
	DeferredRendering(
		MapUtils* minfo,
		DeferredGraphics* defer_graphic
	);
	~DeferredRendering();
	void Render(
		Mat const& cam_view,
		Mat const& cam_proj,
		vec3 const& cam_pos,
		Mat const& s_view,
		Mat const& s_proj
	);

private:
	void setPipe();

private:
	shared_ptr<Buffer<VertexDeffer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	MapUtils* m_info = nullptr;
	ShadowRender s_render;
	GeoRender g_render;
	InputLayouts layout;
	Ssao ssao;
	DeferredGraphics* d_graphic;
	shared_ptr<Texture> texture;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<BlendState> blend_state;
};

