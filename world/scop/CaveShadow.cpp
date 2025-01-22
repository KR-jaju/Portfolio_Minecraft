#include "pch.h"
#include "CaveShadow.h"

#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "RasterizerState.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "MapUtils.h"
#include "Chunk.h"

CaveShadow::CaveShadow(DeferredGraphics* d_graphic, MapUtils* m_info)
{
	this->d_graphic = d_graphic;
	this->m_info = m_info;
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->device = this->d_graphic->getDevice();
	this->context = this->d_graphic->getContext();
	this->d_buffer->setRTVsAndSRVs(this->device, 
		this->m_info->width, this->m_info->height);
	this->vertex_shader = make_shared<VertexShader>(
		this->device,
		L"CaveShadowVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		this->device,
		InputLayouts::layout_pns.data(),
		InputLayouts::layout_pns.size(),
		this->vertex_shader->getBlob()
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		this->device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->pixel_shader = make_shared<PixelShader>(
		this->device,
		L"CaveShadowPS.hlsl",
		"main",
		"ps_5_0"
	);
	MVP mvp;
	this->c_buff = make_shared<ConstantBuffer>(
		this->device,
		this->d_graphic->getContext(),
		mvp
	);
}

void CaveShadow::render(Mat const& cam_view, Mat const& cam_proj)
{
	this->setPipe();
	MVP mvp;
	mvp.view = cam_view.Transpose();
	mvp.proj = cam_proj.Transpose();
	this->c_buff->update(mvp);
	this->context->VSSetConstantBuffers(0, 1,
		this->c_buff->getComPtr().GetAddressOf());
	this->d_graphic->renderBegin(this->d_buffer.get());
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->render_flag == false)
				continue;
			this->m_info->chunks[i][j]->setShadowRender(
				this->d_graphic->getContext()
			);
		}
	}
}

ComPtr<ID3D11ShaderResourceView> CaveShadow::getSRV()
{
	return this->d_buffer->getSRV(0);
}

void CaveShadow::setPipe()
{
	this->context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->context->IASetInputLayout(
		this->input_layout->getComPtr().Get());
	this->context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	this->context->RSSetState(this->rasterizer_state->getComPtr().Get());
	this->context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
}
