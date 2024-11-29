#include "pch.h"
#include "EntityRender.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "MapUtils.h"
#include "EntityUtils.h"
#include "RasterizerState.h"
#include "TextureArray.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "ConstantBuffer.h"
#include "Chunk.h"

EntityRender::EntityRender(
	MapUtils* minfo,
	EntityUtils* einfo,
	DeferredGraphics* dgraphic
)
{
	this->m_info = minfo;
	this->e_info = einfo;
	this->d_graphic = dgraphic;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
		);
	vector<wstring> path_arr = {
		L"grass_top.png",
		L"grass_bottom.png",
		L"grass_side.png"
	};
	this->texture_array = make_shared<TextureArray>(
		device,
		context,
		path_arr,
		0
		);
	//this->texture
	this->sampler_state = make_shared<SamplerState>(device);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"EntityVS.hlsl",
		"main",
		"vs_5_0"
		);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"EntityPS.hlsl",
		"main",
		"ps_5_0"
		);
	this->input_layout = make_shared<InputLayout>(
		device,
		layout.layout_Entity.data(),
		layout.layout_Entity.size(),
		this->vertex_shader->getBlob()
		);
}

void EntityRender::render(
	Mat const& view,
	Mat const& proj,
	vec3 const& cam_pos
)
{
	this->setPipe();
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	MVP mvp;
	mvp.view = view.Transpose();
	mvp.proj = proj.Transpose();
	ConstantBuffer cbuffer(
		this->d_graphic->getDevice(),
		context,
		mvp
	);
	context->VSSetConstantBuffers(0, 1,
		cbuffer.getComPtr().GetAddressOf());
	CamPos cam;
	cam.pos = cam_pos;
	cam.r = 0;
	cam.view = view.Transpose();
	ConstantBuffer cpbuffer(
		this->d_graphic->getDevice(),
		context,
		cam
	);
	context->PSSetConstantBuffers(0, 1,
		cpbuffer.getComPtr().GetAddressOf());
	this->e_info->render(this->d_graphic->getContext());
}

void	EntityRender::setDBuffer(shared_ptr<DeferredBuffer> d_buffer)
{
	this->d_buffer = d_buffer;
}

void EntityRender::setPipe()
{
	ComPtr<ID3D11DeviceContext> context =
		this->d_graphic->getContext();
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
	context->PSSetSamplers(
		0,
		1,
		this->sampler_state->getComPtr().GetAddressOf()
	);
	/*context->PSSetShaderResources(
		0,
		1,
		this->texture_array->getComPtr().GetAddressOf()
	);*/
}
