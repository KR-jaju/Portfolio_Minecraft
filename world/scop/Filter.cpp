#include "pch.h"
#include "Filter.h"
#include "DeferredBuffer.h"
#include "DeferredGraphics.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "Buffer.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "ConstantBuffer.h"
#include "RasterizerState.h"

Filter::Filter(
	DeferredGraphics* dgraphic, 
	UINT width, 
	UINT height,
	wstring const& vs_path,
	wstring const& ps_path
)
{
	this->d_graphic = dgraphic;

	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = width;
	this->view_port.Height = height;
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->d_buffer->setRTVsAndSRVs(
		this->d_graphic->getDevice(),
		width,
		height
	);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		vs_path,
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
		ps_path,
		"main",
		"ps_5_0"
	);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->vbuffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	this->sampler_state = make_shared<SamplerState>(device, desc);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
}

Filter::~Filter()
{
}

void Filter::render()
{
	this->d_graphic->renderBegin(this->d_buffer.get());
	this->setPipe();
	this->d_graphic->setViewPort(this->view_port);
	this->d_graphic->getContext()->PSSetShaderResources(0, 1,
		this->in_srv.GetAddressOf());
	this->d_graphic->getContext()->DrawIndexed(
		ibuffer->getCount(), 0, 0);
}

ComPtr<ID3D11ShaderResourceView> Filter::getSRV()
{
	return this->d_buffer->getSRV(0);
}

void Filter::setStartSRV(ComPtr<ID3D11ShaderResourceView> srv)
{
	this->in_srv = srv;
}

struct cdata
{
	float dx;
	float dy;
	vec2 dummy;
};

void Filter::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	UINT offset = this->vbuffer->getOffset();
	UINT stride = this->vbuffer->getStride();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(), &stride, &offset);
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
	context->PSSetSamplers(0, 1,
		this->sampler_state->getComPtr().GetAddressOf());
	cdata data;
	data.dx = 1.0f / this->view_port.Width;
	data.dy = 1.0f / this->view_port.Height;
	ConstantBuffer cbuffer(
		this->d_graphic->getDevice(),
		context,
		data
	);
	context->PSSetConstantBuffers(0, 1, 
		cbuffer.getComPtr().GetAddressOf());
}
