#include "pch.h"
#include "OITIA.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "Buffer.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "Block.h"

OITIA::OITIA(DeferredGraphics* dgraphics, MapUtils* minfo)
{
	this->d_graphic = dgraphics;
	this->m_info = minfo;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buff = make_shared<DeferredBuffer>(1);
	this->d_buff->setRTVsAndSRVs(
		device,
		this->m_info->width,
		this->m_info->height
	);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->v_buff = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->i_buff = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);

	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"OITIAVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_deferred.data(),
		InputLayouts::layout_deferred.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"OITIAPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);
}

OITIA::~OITIA()
{
}

void OITIA::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	UINT stride = this->v_buff->getStride();
	UINT offset = this->v_buff->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->i_buff->getComPtr().Get(),
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

void OITIA::render(
	ComPtr<ID3D11ShaderResourceView> terrain_srv,
	ComPtr<ID3D11ShaderResourceView> sky_srv
)
{
	this->setPipe();
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->d_graphic->renderBegin(this->d_buff.get());
	context->PSSetShaderResources(0, 1, terrain_srv.GetAddressOf());
	context->PSSetShaderResources(1, 1, sky_srv.GetAddressOf());
	context->DrawIndexed(
		this->i_buff->getCount(),
		0,
		0
	);
}

ComPtr<ID3D11RenderTargetView> OITIA::getRTV()
{
	return this->d_buff->getRTV(0);
}

ComPtr<ID3D11ShaderResourceView> OITIA::getSRV()
{
	return this->d_buff->getSRV(0);
}
