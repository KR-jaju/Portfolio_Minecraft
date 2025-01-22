#include "pch.h"
#include "SsaoRender.h"
#include "MathHelper.h"
#include "DirectXPackedVector.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "Buffer.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "ConstantBuffer.h"

SsaoRender::SsaoRender(DeferredGraphics* d_graphic,
	UINT width, UINT height)
{
	this->d_graphic = d_graphic;
	this->buildOffset();
	this->buildRandomVecTexture();
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->d_buffer->setRTVsAndSRVs(
		device,
		width / 2,
		height / 2
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"SsaoVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"SsaoPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->vertex_shader->getBlob()
	);

	vector<vec3> sample_pos = {
		// front
		{-1.0f, -1.0f, 0.f},
		{-1.0f, +1.0f, 0.f},
		{+1.0f, +1.0f, 0.f},
		{+1.f, -1.f, 0.f},
	};
	vector<vec2> sample_uv = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},
	};
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	VertexDefer v_deff;
	for (int i = 0; i < 4; i++) {
		v_deff.pos = sample_pos[i];
		v_deff.uv = sample_uv[i];
		vertices.push_back(v_deff);
	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
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
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = width / 2;
	this->view_port.Height = height / 2;
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
	MVP mvp;
	this->cbuffer = make_shared<ConstantBuffer>(
		device,
		this->d_graphic->getContext(),
		mvp
	);
}

void SsaoRender::render(Mat const& cam_view, Mat const& cam_proj)
{
	this->setPipe();
	this->d_graphic->setViewPort(this->view_port);
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	MVP mvp;
	mvp.view = cam_view.Transpose();
	mvp.proj = cam_proj.Transpose();
	this->cbuffer->update(mvp);
	context->PSSetConstantBuffers(0, 1, 
		this->cbuffer->getComPtr().GetAddressOf());
	context->PSSetShaderResources(
		3,
		1,
		this->random_vec_SRV.GetAddressOf()
	);
	context->DrawIndexed(
		this->ibuffer->getCount(),
		0,
		0
	);
}

ComPtr<ID3D11ShaderResourceView> SsaoRender::getSRV()
{
	return this->d_buffer->getSRV(0);
}

shared_ptr<DeferredBuffer> SsaoRender::getDBuffer()
{
	return this->d_buffer;
}


void SsaoRender::setPipe()
{
	ComPtr<ID3D11DeviceContext> context =
		this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(
		0,
		1,
		this->vbuffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
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
	context->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);
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
	ConstantBuffer cbuffer(
		this->d_graphic->getDevice(),
		context,
		this->mOffsets
	);
	context->PSSetConstantBuffers(1, 1, cbuffer.getComPtr().GetAddressOf());
}

void SsaoRender::buildOffset()
{
	mOffsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	mOffsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	mOffsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	mOffsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	mOffsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	mOffsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	mOffsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	mOffsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = MathHelper::RandF(0.25f, 1.0f);

		XMVECTOR v = s * XMVector4Normalize(XMLoadFloat4(&mOffsets[i]));

		XMStoreFloat4(&mOffsets[i], v);
	}
}

void SsaoRender::buildRandomVecTexture()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = 256;
	desc.Height = 256;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = 256 * sizeof(DirectX::PackedVector::XMCOLOR);

	DirectX::PackedVector::XMCOLOR color[256 * 256];
	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			XMFLOAT3 v(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());

			color[i * 256 + j] = DirectX::PackedVector::XMCOLOR(v.x, v.y, v.z, 0.0f);
		}
	}

	initData.pSysMem = color;

	ComPtr<ID3D11Texture2D> tex = 0;
	HRESULT hr;
	hr = this->d_graphic->getDevice()->CreateTexture2D(&desc, &initData, tex.GetAddressOf());
	CHECK(hr);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(view_desc));
	view_desc.Format = desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MipLevels = 1;
	hr = this->d_graphic->getDevice()->CreateShaderResourceView(tex.Get(), &view_desc, 
		this->random_vec_SRV.GetAddressOf());
	CHECK(hr);
}
