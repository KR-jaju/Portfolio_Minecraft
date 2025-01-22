#include "pch.h"
#include "ParallaxMapping.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "Block.h"
#include "ConstantBuffer.h"
#include "Buffer.h"
#include "SamplerState.h"

ParallaxMapping::ParallaxMapping(
	DeferredGraphics* graphic, UINT width, UINT height)
{
	this->d_graphic = graphic;
	this->d_buffer = make_shared<DeferredBuffer>(5);
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buffer->setRTVsAndSRVs(device, width, height);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"ParallaxVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"ParallaxPS.hlsl",
		"main",
		"ps_5_0"
	);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1.0f, vertices, indices);
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
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->vertex_shader->getBlob()
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);
	this->eye_poe_cbuffer = make_shared<ConstantBuffer>(
		device,
		this->d_graphic->getContext(),
		vec4(0, 0, 0, 0)
	);
}

ParallaxMapping::~ParallaxMapping()
{
}

void ParallaxMapping::setRTV()
{
	this->d_graphic->renderBegin(this->d_buffer.get());
}

void ParallaxMapping::render(vec3 const& cam_pos)
{
	this->eye_poe_cbuffer->
		update(vec4(cam_pos.x, cam_pos.y, cam_pos.z, 1));
	this->setPipe();
	this->d_graphic->getContext()->DrawIndexed(
		this->ibuffer->getCount(), 0, 0);
}

ComPtr<ID3D11ShaderResourceView> ParallaxMapping::getSRV(RTVIndex idx)
{
	return this->d_buffer->getSRV(static_cast<int>(idx));
}

ComPtr<ID3D11RenderTargetView> ParallaxMapping::getRTV(RTVIndex idx)
{
	return this->d_buffer->getRTV(static_cast<int>(idx));
}

void ParallaxMapping::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT, 0);

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

	context->PSSetConstantBuffers(0, 1,
		this->eye_poe_cbuffer->getComPtr().GetAddressOf());
}
