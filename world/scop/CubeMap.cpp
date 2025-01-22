#include "pch.h"
#include "CubeMap.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Block.h"
#include "Buffer.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "ConstantBuffer.h"
#include "SunMoon.h"
#include "Texture.h"

CubeMap::CubeMap(
	DeferredGraphics* dgraphic,
	UINT width, 
	UINT height
)
{
	this->d_graphic = dgraphic;
	this->width = width;
	this->height = height;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->d_buffer->setRTVsAndSRVs(device, width, height);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"SkyBoxVS.hlsl",
		"main",
		"vs_5_0"
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
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"SkyBoxPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->sampler_state = make_shared<SamplerState>(device);
	this->cube_tex = make_shared<Texture>(
		device,
		L"./textures/skybox/HDRI/MyCubesEnvHDR.dds",
		true
	);
}

void CubeMap::render(
	Mat const& cam_view, 
	Mat const& cam_proj,
	vec3 const& cam_pos
)
{
	this->d_graphic->renderBegin(this->d_buffer.get());
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeCubeMap(
		300.f,
		vertices,
		indices
	);
	this->vbuffer = make_shared<Buffer<VertexDefer>>(
		this->d_graphic->getDevice(),
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		this->d_graphic->getDevice(),
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->setPipe();
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	MVP mvp;
	mvp.model = XMMatrixTranslation(cam_pos.x, cam_pos.y, cam_pos.z);
	mvp.model = mvp.model.Transpose();
	mvp.view = cam_view.Transpose();
	mvp.proj = cam_proj.Transpose();
	ConstantBuffer cbuffer(device, context, mvp);
	context->VSSetConstantBuffers(0, 1,
		cbuffer.getComPtr().GetAddressOf());
	context->DrawIndexed(this->ibuffer->getCount(), 0, 0);
}

ComPtr<ID3D11ShaderResourceView> CubeMap::getSRV()
{
	return this->d_buffer->getSRV(0);
}


void CubeMap::setPipe()
{
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	UINT offset = this->vbuffer->getOffset();
	UINT stride = this->vbuffer->getStride();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->IASetIndexBuffer(
		this->ibuffer->getComPtr().Get(),
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
	context->PSSetShaderResources(0, 1, 
		this->cube_tex->getComPtr().GetAddressOf());
	context->PSSetSamplers(0, 1,
		this->sampler_state->getComPtr().GetAddressOf());
}
