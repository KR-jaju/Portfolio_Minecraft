#include "pch.h"
#include "ResultSM.h"
#include "SunMoon.h"
#include "Blur.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "Buffer.h"
#include "Block.h"
#include "SamplerState.h"
#include "RasterizerState.h"

ResultSM::ResultSM(
	DeferredGraphics* graphic, 
	UINT width, 
	UINT height
)
{
	this->d_graphic = graphic;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->d_buffer->setRTVsAndSRVs(device, width, height);
	this->sun_moon = make_shared<SunMoon>(graphic, width, height);
	this->blur = make_shared<Blur>(graphic, width, height);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
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
	this->vertex_sahder = make_shared<VertexShader>(
		device,
		L"SemiCombineVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->rasterizer_sate = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"SemiCombinePS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->vertex_sahder->getBlob()
	);
	this->sampler_state = make_shared<SamplerState>(device);
}

void ResultSM::render(
	vec3 const& cam_pos,
	Mat const& cam_view,
	Mat const& cam_proj
)
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();	
	this->sun_moon->render(cam_pos, cam_view, cam_proj);
	this->blur->setStartSRV(this->sun_moon->getSRV());
	this->blur->render();
	this->setPipe();
	this->d_graphic->renderBegin(this->d_buffer.get());
	context->PSSetShaderResources(0, 1,
		this->sun_moon->getSRV().GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->blur->getSRV().GetAddressOf());
	context->DrawIndexed(this->ibuffer->getCount(), 0, 0);
}

void ResultSM::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	UINT stride = this->vbuffer->getStride();
	UINT offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->VSSetShader(
		this->vertex_sahder->getComPtr().Get(),
		nullptr,
		0
	);
	context->RSSetState(this->rasterizer_sate->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->PSSetSamplers(0, 1,
		this->sampler_state->getComPtr().GetAddressOf());
}

ComPtr<ID3D11ShaderResourceView> ResultSM::getSRV()
{
	return this->d_buffer->getSRV(0);
}

vec3 ResultSM::getDirectionalLightPos()
{
	return this->sun_moon->getLightPos();
}
