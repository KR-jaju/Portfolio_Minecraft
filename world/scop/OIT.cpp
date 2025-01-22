#include "pch.h"
#include "OIT.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "DeferredBuffer.h"
#include "DeferredGraphics.h"
#include "Block.h"
#include "Buffer.h"
#include "SamplerState.h"

OIT::OIT(
	DeferredGraphics *graphic,
	MapUtils* minfo
) 
	: d_graphic(graphic), m_info(minfo), d_buff(nullptr), tp(graphic, minfo),
	cp(graphic, minfo)
{
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buff = make_shared<DeferredBuffer>(1);
	this->d_buff->setRTVsAndSRVs(device,
		this->m_info->width, this->m_info->height);

	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"OITResultVS.hlsl",
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
		L"OITResultPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->v_buff = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->i_buff = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
}

OIT::~OIT()
{
}

void OIT::setRTVandSRV(
	ComPtr<ID3D11RenderTargetView> rtv, 
	ComPtr<ID3D11ShaderResourceView> srv,
	ComPtr<ID3D11ShaderResourceView> depth_srv
)
{
	this->solid_rtv = rtv;
	this->solid_srv = srv;
	this->depth_srv = depth_srv;
}

void OIT::setPipe()
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
}

void OIT::render(Mat const& cam_view, Mat const& cam_proj)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	
	// transparent render
	this->tp.render(cam_view, cam_proj, this->depth_srv);

	// composition render
	this->cp.render(this->solid_rtv, this->tp.getAccum(), this->tp.getReveal());

	// result render
	this->setPipe();
	UINT stride = this->v_buff->getStride();
	UINT offset = this->v_buff->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->i_buff->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	this->d_graphic->renderBegin(this->d_buff.get());
	context->PSSetShaderResources(0, 1, this->solid_srv.GetAddressOf());
	context->DrawIndexed(
		this->i_buff->getCount(),
		0, 0);
}

ComPtr<ID3D11ShaderResourceView> OIT::getSRV()
{
	return this->d_buff->getSRV(0);
}
