#include "pch.h"
#include "PBR.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "Block.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "RasterizerState.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "ConstantBuffer.h"
#include "Buffer.h"
#include "Texture.h"

struct PS_Constant
{
	vec3 cam_pos;
	int dummy1;
	vec3 light_pos;
	int dummy2;
};

PBR::PBR(DeferredGraphics* grpahic, UINT width, UINT height)
{
	this->d_graphic = grpahic;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buffer = make_shared<DeferredBuffer>(2);
	this->d_buffer->setRTVsAndSRVs(device, width, height);
	vector<VertexDefer> vertices;
	vector<uint32> Indices;
	Block::makeBox(1, vertices, Indices);
	this->vbuffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		device,
		Indices.data(),
		Indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"PBRVS.hlsl",
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
		L"PBRPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->sampler_state_linear = make_shared<SamplerState>(device);
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	this->sampler_state_clamp = make_shared<SamplerState>(device, desc);
	this->irradiance_tex = make_shared<Texture>(
		device,
		L"./textures/skybox/HDRI/MyCubesDiffuseHDR.dds",
		true
	);
	this->specular_tex = make_shared<Texture>(
		device,
		L"./textures/skybox/HDRI/MyCubesSpecularHDR.dds",
		true
	);
	this->brdf_tex = make_shared<Texture>(
		device,
		L"./textures/skybox/HDRI/MyCubesBRDF.dds",
		false
	);
	PS_Constant buffer;
	this->cbuffer = make_shared<ConstantBuffer>(
		device, this->d_graphic->getContext(), buffer);
}

PBR::~PBR()
{
}

void PBR::setRTV()
{
	this->d_graphic->renderBegin(this->d_buffer.get());
}

void PBR::render(
	vec3 const& light_pos, 
	vec3 const& cam_pos,
	ComPtr<ID3D11ShaderResourceView> color_srv
)
{
	this->setCBuffer(light_pos, cam_pos);
	this->setPipe();
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->PSSetShaderResources(0, 1, color_srv.GetAddressOf());
	context->DrawIndexed(this->ibuffer->getCount(), 0, 0);
}

ComPtr<ID3D11ShaderResourceView> PBR::getAmbientLight()
{
	return this->d_buffer->getSRV(0);
}

ComPtr<ID3D11ShaderResourceView> PBR::getDirectLight()
{
	return this->d_buffer->getSRV(1);
}

void PBR::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(
		this->input_layout->getComPtr().Get());
	uint32 offset = this->vbuffer->getOffset();
	uint32 stride = this->vbuffer->getStride();
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
		nullptr, 0);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr, 0);
	context->PSSetShaderResources(4, 1,
		this->irradiance_tex->getComPtr().GetAddressOf());
	context->PSSetShaderResources(5, 1,
		this->specular_tex->getComPtr().GetAddressOf());
	context->PSSetShaderResources(6, 1,
		this->brdf_tex->getComPtr().GetAddressOf());
	context->PSSetSamplers(0, 1,
		this->sampler_state_linear->getComPtr().GetAddressOf());
	context->PSSetSamplers(1, 1,
		this->sampler_state_clamp->getComPtr().GetAddressOf());
}

void PBR::setCBuffer(vec3 const& light_pos, vec3 const& cam_pos)
{
	PS_Constant buffer;
	buffer.cam_pos = cam_pos;
	buffer.light_pos = light_pos;
	this->cbuffer->update(buffer);
	this->d_graphic->getContext()->PSSetConstantBuffers(0, 1,
		this->cbuffer->getComPtr().GetAddressOf());
}
