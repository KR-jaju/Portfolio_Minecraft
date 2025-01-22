#include "pch.h"
#include "Transparent.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "RasterizerState.h"
#include "Chunk.h"
#include "MapUtils.h"
#include "SamplerState.h"

#include "BlendState.h"

Transparent::Transparent(
	DeferredGraphics* d_graphic, 
	MapUtils* minfo
)
{
	this->d_graphic = d_graphic;
	this->m_info = minfo;
	this->d_buffer = make_shared<DeferredBuffer>(2);
	this->d_buffer->setRTVsAndSRVs(this->d_graphic->getDevice(), 
		this->m_info->width, this->m_info->height);

	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();

	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(blend_desc));
	blend_desc.AlphaToCoverageEnable = false;
	blend_desc.IndependentBlendEnable = true;

	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;

	blend_desc.RenderTarget[1].BlendEnable = true;
	blend_desc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[1].SrcBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_COLOR;

	blend_desc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[1].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = device->CreateBlendState(&blend_desc,
		this->blend_state.GetAddressOf());

	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"TransparentVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pc.data(),
		InputLayouts::layout_pc.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"TransparentPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	MVP mvp;
	this->constant_buffer = make_shared<ConstantBuffer>(
		device,
		this->d_graphic->getContext(),
		mvp
	);

	this->d_graphic->getContext()->OMGetDepthStencilState(
		this->prev_ds_state.GetAddressOf(),
		&(this->prev_ref)
	);
	D3D11_DEPTH_STENCIL_DESC ds_desc;
	ZeroMemory(&ds_desc, sizeof(ds_desc));
	ds_desc.DepthEnable = false;
	ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	ds_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	ds_desc.StencilEnable = false;
	hr = device->CreateDepthStencilState(&ds_desc, this->ds_state.GetAddressOf());
	CHECK(hr);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	this->sampler_state = make_shared<SamplerState>(device, sampler_desc);
}

Transparent::~Transparent()
{
}

void Transparent::setPipe()
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
	context->OMSetDepthStencilState(this->ds_state.Get(), 0);
}

void Transparent::render(
	Mat const& cam_view, 
	Mat const& cam_proj,
	ComPtr<ID3D11ShaderResourceView> depth_srv
)
{
	this->setPipe();
	MVP mvp;
	mvp.view = cam_view.Transpose();
	mvp.proj = cam_proj.Transpose();
	this->constant_buffer->update(mvp);
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->d_graphic->renderBegin(this->d_buffer.get());
	context->VSSetConstantBuffers(0, 1,
		this->constant_buffer->getComPtr().GetAddressOf());
	context->ClearRenderTargetView(
		this->d_buffer->getRTV(0).Get(),
		this->clear_accum
	);
	context->ClearRenderTargetView(
		this->d_buffer->getRTV(1).Get(),
		this->clear_reveal
	);

	context->PSSetShaderResources(0, 1, depth_srv.GetAddressOf());
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->tp_chunk.render_flag == false)
				continue;
			this->m_info->chunks[i][j]->tp_chunk.setTPBuffer(context);
		}
	}
	context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(this->prev_ds_state.Get(), this->prev_ref);
}

ComPtr<ID3D11ShaderResourceView> Transparent::getAccum()
{
	return this->d_buffer->getSRV(0);
}

ComPtr<ID3D11ShaderResourceView> Transparent::getReveal()
{
	return this->d_buffer->getSRV(1);
}
