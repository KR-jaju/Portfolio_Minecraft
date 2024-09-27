#include "pch.h"
#include "RenderSystem.h"
#include "TextureArray.h"
#include "MapUtils.h"
#include "Chunk.h"
#include "InputLayouts.h"

#include "Graphics.h"
#include "RasterizerState.h"
#include "InputLayout.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Buffer.h"

RenderSystem::RenderSystem(MapUtils* minfo)
{
	this->m_info = minfo;
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::setGraphic(shared_ptr<Graphics> graphic)
{
	this->graphic = graphic;
}

void RenderSystem::setRender()
{
	vector<wstring> path_arr = {
		L"grass_top.png",
		L"grass_bottom.png",
		L"grass_side.png"
	};
	this->texture_array = make_shared<TextureArray>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		path_arr,
		0
	);
	this->blend_state_arr.push_back(
		make_shared<BlendState>(this->graphic->getDevice())
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = 
		make_shared<SamplerState>(this->graphic->getDevice());
	this->vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"TestVertexShader.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout.layout_0.data(),
		layout.layout_0.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"TestPixelShader.hlsl",
		"main",
		"ps_5_0"
	);
}

void RenderSystem::setDepthRender()
{
	this->depth_rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_FRONT
	);
	this->depth_vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"DepthVertexShader.hlsl",
		"main",
		"vs_5_0"
	);
	this->depth_input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		this->layout.layout_0.data(),
		this->layout.layout_0.size(),
		this->depth_vertex_shader->getBlob()
	);
	this->depth_pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"DepthPixelShader.hlsl",
		"main",
		"ps_5_0"
	);
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.BorderColor[0] = 0.f;
	comparisonSamplerDesc.BorderColor[1] = 0.f;
	comparisonSamplerDesc.BorderColor[2] = 0.f;
	comparisonSamplerDesc.BorderColor[3] = 0.f;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	comparisonSamplerDesc.MinLOD = 0;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	this->depth_sampler_state = make_shared<SamplerState>(
		this->graphic->getDevice(),
		comparisonSamplerDesc
	);
}

void RenderSystem::setDepthPipeLine()
{
	this->graphic->getContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	this->graphic->getContext()->IASetInputLayout(
		this->depth_input_layout->getComPtr().Get()
	);

	this->graphic->getContext()->VSSetShader(
		this->depth_vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	this->graphic->getContext()->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);
	this->graphic->getContext()->PSSetShader(
		this->depth_pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
}

void RenderSystem::setPipeLine()
{
	this->graphic->getContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	this->graphic->getContext()->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	this->graphic->getContext()->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	this->graphic->getContext()->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);
	this->graphic->getContext()->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
	this->graphic->getContext()->PSSetSamplers(
		0,
		1,
		this->sampler_state->getComPtr().GetAddressOf()
	);
	if (this->depth_sampler_state) {
		this->graphic->getContext()->PSSetSamplers(
			1,
			1,
			this->depth_sampler_state->getComPtr().GetAddressOf()
		);
	}
	this->graphic->getContext()->PSSetShaderResources(
		0,
		1,
		this->texture_array->getComPtr().GetAddressOf()
	);
	this->graphic->getContext()->OMSetBlendState(
		this->blend_state_arr[0]->getComPtr().Get(),
		this->blend_state_arr[0]->getBlendFactor(),
		this->blend_state_arr[0]->getSampleMask()
	);
}

void RenderSystem::Render(Mat const& cam_view, Mat const& cam_proj, vec3 const& cam_pos)
{
	this->graphic->renderBegin();
	this->setPipeLine();
	CamPos cam;
	cam.pos = cam_pos;
	cam.r = 16.f * 1.f;
	MVP mvp;
	mvp.proj = cam_proj.Transpose();
	mvp.view = cam_view.Transpose();
	ConstantBuffer vertex_cbuffer(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		mvp
	);
	this->graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		vertex_cbuffer.getComPtr().GetAddressOf()
	);
	ConstantBuffer pixel_cbuffer(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		cam
	);
	this->graphic->getContext()->PSSetConstantBuffers(
		0,
		1,
		pixel_cbuffer.getComPtr().GetAddressOf()
	);
	this->graphic->setClearColor(0.3, 0.3, 0.3, 1);
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->render_flag == false) {
				continue;
			}
			this->m_info->chunks[i][j]->setRender(
				this->graphic,
				this->vertex_shader,
				this->render_flag
			);
		}
	}
	this->graphic->renderEnd();
}

void RenderSystem::depthRender()
{
	this->setDepthPipeLine();
	D3D11_VIEWPORT view_port;
	ZeroMemory(&view_port, sizeof(view_port));
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	int times = 10;
	view_port.Width = pow(2, times);
	view_port.Height = pow(2, times);
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	this->graphic->getContext()->RSSetViewports(1, &(view_port));
	this->render_flag = 1;
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->render_flag == false) {
				continue;
			}
			this->m_info->chunks[i][j]->setDepthRender(
				this->graphic,
				this->depth_vertex_shader
			);
		}
	}
}


