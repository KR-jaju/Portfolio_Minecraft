#include "pch.h"
#include "GeoRender.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "MapUtils.h"
#include "RasterizerState.h"
#include "TextureArray.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "ConstantBuffer.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "Chunk.h"

GeoRender::GeoRender(
	MapUtils* minfo,
	DeferredGraphics* dgraphic
)
{
	this->m_info = minfo;
	this->d_graphic = dgraphic;
	this->d_buffer = make_shared<DeferredBuffer>(3);
	this->d_buffer->setRTVsAndSRVs(
		this->d_graphic->getDevice(),
		this->m_info->width,
		this->m_info->height
	);
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		//D3D11_FILL_SOLID,
		D3D11_FILL_WIREFRAME,
		D3D11_CULL_BACK
	);
	vector<wstring> path_arr = {
		L"./textures/blocks/grass_top.png",
		L"./textures/blocks/grass_bottom.png",
		L"./textures/blocks/grass_side.png"
	};
	this->texture_array = make_shared<TextureArray>(
		device,
		context,
		path_arr,
		0
	);
	this->sampler_state = make_shared<SamplerState>(device);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"GeometryVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"GeometryPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_Geo.data(),
		InputLayouts::layout_Geo.size(),
		this->vertex_shader->getBlob()
	);
	this->hull_shader = make_shared<HullShader>(
		device,
		L"GeometryHS.hlsl",
		"main",
		"hs_5_0"
	);
	this->domain_shader = make_shared<DomainShader>(
		device,
		L"GeometryDS.hlsl",
		"main",
		"ds_5_0"
	);
}

GeoRender::~GeoRender()
{
}

void GeoRender::render(
	Mat const& view, 
	Mat const& proj,
	vec3 const& cam_pos
)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_graphic->renderBegin(this->d_buffer.get());
	this->setPipe();
	MVP mvp;
	mvp.view = view.Transpose();
	mvp.proj = proj.Transpose();
	ConstantBuffer cbuffer(device, context, mvp);
	context->VSSetConstantBuffers(0, 1,
		cbuffer.getComPtr().GetAddressOf());
	CamPos cam;
	cam.pos = cam_pos;
	cam.r = 0;
	cam.view = view.Transpose();
	ConstantBuffer cpbuffer(device, context, cam);
	vec4 test_eye;
	test_eye.x = cam_pos.x;
	test_eye.y = cam_pos.y;
	test_eye.z = cam_pos.z;
	ConstantBuffer chbuffer(device, context, test_eye);
	context->HSSetConstantBuffers(0, 1,
		chbuffer.getComPtr().GetAddressOf());
	context->DSSetConstantBuffers(0, 1,
		cbuffer.getComPtr().GetAddressOf());
	context->PSSetConstantBuffers(0, 1, 
		cpbuffer.getComPtr().GetAddressOf());
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->render_flag == false)
				continue;
			this->m_info->chunks[i][j]->setGeoRender(
				this->d_graphic->getContext(),
				this->vertex_shader
			);
		}
	}
	context->HSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
}

ComPtr<ID3D11ShaderResourceView> GeoRender::getSRV(int idx)
{
	return this->d_buffer->getSRV(idx);
}

void GeoRender::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	/*context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);*/
	context->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
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
	context->PSSetShaderResources(
		0,
		1,
		this->texture_array->getComPtr().GetAddressOf()
	);
	context->HSSetShader(
		this->hull_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->DSSetShader(
		this->domain_shader->getComPtr().Get(),
		nullptr,
		0
	);
}
