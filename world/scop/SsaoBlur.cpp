#include "pch.h"
#include "SsaoBlur.h"
#include "DeferredGraphics.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "DeferredBuffer.h"

struct SsaoBlurBuffer
{
	float texel_width;
	float texel_height;
	int wh_flag;
	int dummy;
};

SsaoBlur::SsaoBlur(
	DeferredGraphics* d_graphic, 
	UINT width, UINT height
)
{
	this->d_graphic = d_graphic;
	this->width = width;
	this->height = height;
	ComPtr<ID3D11Device> device = d_graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"SsaoBlurVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		layout.layout_deferred.data(),
		layout.layout_deferred.size(),
		this->vertex_shader->getBlob()
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"SsaoBlurPS.hlsl",
		"main",
		"ps_5_0"
	);
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
	this->d_buffer_width = make_shared<DeferredBuffer>(1);
	this->d_buffer_width->setRTVsAndSRVs(
		device,
		width,
		height
	);
	this->d_buffer_height = make_shared<DeferredBuffer>(1);
	this->d_buffer_height->setRTVsAndSRVs(
		device,
		width,
		height
	);
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = width;
	this->view_port.Height = height;
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
}

void SsaoBlur::render(int wh_flag)
{
	this->setPipe();
	this->d_graphic->setViewPort(this->view_port);
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	SsaoBlurBuffer sb;
	sb.texel_height = 1.0f / (this->height);
	sb.texel_width = 1.0f / (this->width);
	sb.wh_flag = wh_flag;
	ConstantBuffer cbuffer(device, context, sb);
	context->PSSetConstantBuffers(0, 1,
		cbuffer.getComPtr().GetAddressOf());
	context->DrawIndexed(
		this->ibuffer->getCount(),
		0,
		0
	);
	context->Flush();
}

ComPtr<ID3D11ShaderResourceView> SsaoBlur::getWidthSRV()
{
	return this->d_buffer_width->getSRV(0);
}

ComPtr<ID3D11ShaderResourceView> SsaoBlur::getHeightSRV()
{
	return this->d_buffer_height->getSRV(0);
}

shared_ptr<DeferredBuffer> const& SsaoBlur::getWidthDBuffer()
{
	return this->d_buffer_width;
}

shared_ptr<DeferredBuffer> const& SsaoBlur::getHeightDBuffer()
{
	return this->d_buffer_height;
}

void SsaoBlur::setPipe()
{
	ComPtr<ID3D11DeviceContext> context =
		this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(
		0,
		1,
		this->vbuffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
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
	context->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
}
