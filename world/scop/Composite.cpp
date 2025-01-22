#include "pch.h"
#include "Composite.h"
#include "DeferredGraphics.h"
#include "MapUtils.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "ConstantBuffer.h"
#include "Buffer.h"
#include "Block.h"
#include "SamplerState.h"

Composite::Composite(
	DeferredGraphics* dgraphic, 
	MapUtils* minfo
)
{
	this->d_graphic = dgraphic;
	this->m_info = minfo;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->v_buffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->i_buffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"CompositeVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"CompositePS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);

	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(blend_desc));
	blend_desc.AlphaToCoverageEnable = false;
	blend_desc.IndependentBlendEnable = false;

	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = device->CreateBlendState(&blend_desc, 
		this->blend_state.GetAddressOf());
}

Composite::~Composite()
{
}

void Composite::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
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
	context->OMSetBlendState(
		this->blend_state.Get(),
		nullptr,
		0xFFFFFFFF
	);
}

void Composite::render( 
	ComPtr<ID3D11RenderTargetView> solid_rtv,
	ComPtr<ID3D11ShaderResourceView> accum,
	ComPtr<ID3D11ShaderResourceView> reveal
)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->setPipe();
	UINT stride = this->v_buffer->getStride();
	UINT offset = this->v_buffer->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->v_buffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->i_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);

	context->OMSetRenderTargets(1, solid_rtv.GetAddressOf(), nullptr);
	context->PSSetShaderResources(0, 1, accum.GetAddressOf());
	context->PSSetShaderResources(1, 1, reveal.GetAddressOf());
	context->DrawIndexed(
		this->i_buffer->getCount(),
		0, 0);
	context->OMSetBlendState(nullptr, nullptr, 0XFFFFFFFF);
}
