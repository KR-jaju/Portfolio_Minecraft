#include "pch.h"
#include "ShadowRender.h"
#include "MapUtils.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "ConstantBuffer.h"
#include "Chunk.h"
#include "BlendState.h"
#include "DepthMap.h"

ShadowRender::ShadowRender(
	MapUtils* minfo,
	DeferredGraphics* dgraphic
)
{
	this->m_info = minfo;
	this->d_graphic = dgraphic;
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->d_buffer->setRTVsAndSRVs(
		this->d_graphic->getDevice(),
		this->m_info->width,
		this->m_info->height
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		this->d_graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->vertex_shader = make_shared<VertexShader>(
		this->d_graphic->getDevice(),
		L"ShadowVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		this->d_graphic->getDevice(),
		L"ShadowPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		this->d_graphic->getDevice(),
		InputLayouts::layout_shadow.data(),
		InputLayouts::layout_shadow.size(),
		this->vertex_shader->getBlob()
	);
	this->blend_state = make_shared<BlendState>(
		this->d_graphic->getDevice()
	);
	this->depth_map = make_shared<DepthMap>(
		this->d_graphic->getDevice()
	);
}

ShadowRender::~ShadowRender()
{
}

void ShadowRender::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	context->OMSetBlendState(
		this->blend_state->getComPtr().Get(),
		this->blend_state->getBlendFactor(),
		this->blend_state->getSampleMask()
	);
}

void ShadowRender::render(
	Mat const& view,
	Mat const& proj
)
{
	this->setPipe();
	ComPtr<ID3D11DeviceContext> context =
		this->d_graphic->getContext();
	MVP mvp;
	mvp.proj = proj.Transpose();
	mvp.view = view.Transpose();
	ConstantBuffer cbuffer(
		this->d_graphic->getDevice(),
		context,
		mvp
	);
	this->d_graphic->renderBegin(
		this->d_buffer.get(),
		this->depth_map->getDepthStencilView()
	);
	context->VSSetConstantBuffers(
		0, 1, cbuffer.getComPtr().GetAddressOf());
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->render_flag == false)
				continue;
			this->m_info->chunks[i][j]->setShadowRender(
				this->d_graphic->getContext(),
				this->vertex_shader
			);
		}
	}
}

ComPtr<ID3D11ShaderResourceView> ShadowRender::getSRV()
{
	return this->d_buffer->getSRV(0);
}

ComPtr<ID3D11ShaderResourceView> ShadowRender::getDepthSRV()
{
	return this->depth_map->getShaderResourceView();
}
