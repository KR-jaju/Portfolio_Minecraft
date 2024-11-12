#pragma once

#include "ShadowRender.h"
#include "GeoRender.h"
#include "EntityRender.h"
#include "WorldUtils.h"
#include "BlendState.h"
#include "SsaoRender.h"
#include "SsaoBlur.h"

class DeferredGraphics;
class DeferredBuffer;
class MapUtils;
class EntityUtils;
template <typename T> class Buffer;
class Texture;

class DeferredRendering
{
public:
	DeferredRendering(
		MapUtils* minfo,
		EntityUtils* einfo,
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
	void ssaoBlur(int cnt, Mat const& proj);

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	MapUtils* m_info = nullptr;
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	ShadowRender s_render;
	GeoRender g_render;
	EntityRender e_render;
	SsaoRender ssao_render;
	SsaoBlur ssao_blur;

private:
	InputLayouts layout;
	D3D11_VIEWPORT view_port;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<InputLayout> input_layout;
};

