#pragma once

#include "CascadeShadow.h"
#include "WorldUtils.h"

class MapUtils;
template<typename T> class Buffer;


struct FrusumSplit {
	vec4 light_pos; // world space
	float vz_arr[8]; // view space
	Mat view;
};

class ShadowRender
{
public:
	ShadowRender(
		MapUtils* minfo,
		DeferredGraphics* dgraphic
	);
	~ShadowRender();
	void renderCSM(
		Mat const& cam_view,
		Mat const& cam_proj
	);
	void render(
		Mat const& cam_view,
		Mat const& cam_proj
	);
	ComPtr<ID3D11ShaderResourceView> getSRV();

private:
	void setPipe();
	void devideFrustum();

private:
	MapUtils* m_info;
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<ConstantBuffer> ps_cbuffer;

private:
	int split_cnt = 3;
	FrusumSplit frustum_split;
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	vector<MVP> mvps;
	vector<FrustumVertex> f_vertices;
	shared_ptr<CascadeShadow> csm;
	shared_ptr<StructuredBuffer> structured_buffer;
};

