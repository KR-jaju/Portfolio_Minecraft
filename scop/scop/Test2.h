#pragma once

#include "Buffer.h"
#include "InputLayout.h"
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "Context.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "SwapChain.h"

class Test2
{
public:
	Test2(HWND hwnd, UINT width, UINT height);
	~Test2();
	void setDrawTexSkel();
	void render();
	void renderUV();
	void update();
	void setContext();
private:
	void geometrySkel();
	void geometryBox();

private:
	// 밖에서 정의됨
	shared_ptr<Graphics> graphic;
	Context	context;

	// 실제 object들이 render 하기 위해 갖고 있을 부분
	shared_ptr<Buffer<Vertex>> vertex_buffer;
	shared_ptr<Buffer<VertexUV>> vertex_uv_buffer;
	shared_ptr<Buffer<uint32>> index_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<Texture> texture;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<BlendState> blend_state;

	SwapChain			swapchain;
	RenderTargetView	swapchain_rtv;
	DepthStencilTexture	depth_texture;
	DepthStencilView	dsv;

	vector<uint32> indices; // quad의 indices
	vector<VertexUV> vertices_uv; // quad의 모양 정의
};

