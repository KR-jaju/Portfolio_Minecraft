#include "pch.h"
#include "DeferredRendering.h"
#include "MapUtils.h"
#include "DeferredGraphics.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Buffer.h"
#include "SamplerState.h"


DeferredRendering::DeferredRendering(
	MapUtils* minfo,
	DeferredGraphics* defer_graphic
)
	: m_info(minfo), s_render(minfo, defer_graphic), 
	g_render(minfo, defer_graphic), ssao_render(defer_graphic, 
		minfo->width, minfo->height), 
	ssao_blur(defer_graphic, minfo->width, minfo->height)
{
	this->d_graphic = defer_graphic;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"ResultVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"ResultPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		this->layout.layout_deferred.data(),
		this->layout.layout_deferred.size(),
		this->vertex_shader->getBlob()
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);
	vector<vec3> sample_pos = {
		// front
		{-1.f, -1.f, 0.f},
		{-1.f, +1.f, 0.f},
		{+1.f, +1.f, 0.f},
		{+1.f, -1.f, 0.f},
	};
	vector<vec2> sample_uv = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},
	};
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	VertexDefer v_deff;
	for (int i = 0; i < 4; i++) {
		v_deff.pos = sample_pos[i];
		v_deff.uv = sample_uv[i];
		vertices.push_back(v_deff);
	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	this->vbuffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = this->m_info->width;
	this->view_port.Height = this->m_info->height;
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
}

DeferredRendering::~DeferredRendering()
{
}

void DeferredRendering::ssaoBlur(int cnt, Mat const& proj)
{
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();
	//// ssao blur width start
	this->d_graphic->renderBegin(
		this->ssao_blur.getWidthDBuffer().get());
	context->PSSetShaderResources(0, 1,
		this->g_render.getSRV(1).GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->s_render.getDepthSRV().GetAddressOf());
	context->PSSetShaderResources(2, 1,
		this->ssao_render.getSRV().GetAddressOf());
	this->ssao_blur.render(0, proj, 2.0f);

	//// ssao blur height start
	this->d_graphic->renderBegin(
		this->ssao_blur.getHeightDBuffer().get());
	context->PSSetShaderResources(0, 1,
		this->g_render.getSRV(1).GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->s_render.getDepthSRV().GetAddressOf());
	context->PSSetShaderResources(2, 1,
		this->ssao_blur.getWidthSRV().GetAddressOf());
	this->ssao_blur.render(1, proj, 1.0f);

	for (int i = 1; i < cnt; i++) {
		// ssao blur width start
		this->d_graphic->renderBegin(
			this->ssao_blur.getWidthDBuffer().get());
		context->PSSetShaderResources(0, 1,
			this->g_render.getSRV(1).GetAddressOf());
		context->PSSetShaderResources(1, 1,
			this->s_render.getDepthSRV().GetAddressOf());
		context->PSSetShaderResources(2, 1,
			this->ssao_blur.getHeightSRV().GetAddressOf());
		this->ssao_blur.render(0, proj, 1.0f);

		// ssao blur height start
		this->d_graphic->renderBegin(
			this->ssao_blur.getHeightDBuffer().get());
		context->PSSetShaderResources(0, 1,
			this->g_render.getSRV(1).GetAddressOf());
		context->PSSetShaderResources(1, 1,
			this->s_render.getDepthSRV().GetAddressOf());
		context->PSSetShaderResources(2, 1,
			this->ssao_blur.getWidthSRV().GetAddressOf());
		this->ssao_blur.render(1, proj, 1.0f);
	}
}


void DeferredRendering::Render(
	Mat const& cam_view, 
	Mat const& cam_proj, 
	vec3 const& cam_pos,
	Mat const& s_view, 
	Mat const& s_proj
)
{
	this->g_render.render(cam_view, cam_proj, cam_pos);
	this->s_render.render(s_view, s_proj);
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();

	// ssao start
	this->d_graphic->renderBegin(this->ssao_render.getDBuffer().get());
	for (int i = 1; i < 3; i++) {
		context->PSSetShaderResources(
			i - 1,
			1,
			this->g_render.getSRV(i).GetAddressOf()
		);
	}
	context->PSSetShaderResources(
		2,
		1,
		this->s_render.getDepthSRV().GetAddressOf()
	);
	this->ssao_render.render(cam_proj);
	
	// ssao blur start
	this->ssaoBlur(4, cam_proj);
	
	// result render start
	this->setPipe();
	this->d_graphic->setViewPort(this->view_port);
	context->PSSetShaderResources(0, 1,
		this->g_render.getSRV(0).GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->s_render.getSRV().GetAddressOf());
	this->d_graphic->renderBegin();
	context->PSSetShaderResources(2, 1, this->ssao_blur.getHeightSRV().GetAddressOf());
	context->DrawIndexed(
		this->ibuffer->getCount(),
		0, 0);
	this->d_graphic->renderEnd();
}

void DeferredRendering::setPipe()
{
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<ID3D11Device> device;
	context = this->d_graphic->getContext();
	device = this->d_graphic->getDevice();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->IASetIndexBuffer(
		this->ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->PSSetSamplers(0, 1,
		this->sampler_state->getComPtr().GetAddressOf());
}