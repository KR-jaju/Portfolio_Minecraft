#include "pch.h"
#include "Test2.h"

Test2::Test2(HWND hwnd, UINT width, UINT height)
	: context(hwnd),
	swapchain(context, width, height),
	swapchain_rtv(this->context, this->swapchain),
	depth_texture(context, width, height),
	dsv(this->context, this->depth_texture)
{
	this->graphic = make_shared<Graphics>(hwnd, width, height);
	
}

Test2::~Test2()
{
}

void Test2::setDrawTexSkel()
{
	this->geometrySkel();
	////this->graphic->setClearColor(0.5f, 0.5f, 0.5f, 1.f);
	//this->index_buffer = make_shared<Buffer<uint32>>(
	//	this->graphic->getDevice(),
	//	this->indices.data(),
	//	(uint32)(this->indices.size()),
	//	D3D11_BIND_INDEX_BUFFER
	//	);
	//this->vertex_uv_buffer = make_shared<Buffer<VertexUV>>(
	//	this->graphic->getDevice(),
	//	this->vertices_uv.data(),
	//	(uint32)(this->vertices_uv.size()),
	//	D3D11_BIND_VERTEX_BUFFER
	//	);
	//vector<D3D11_INPUT_ELEMENT_DESC> layout = {
	//	{
	//		"POSITION",
	//		0,
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		0,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},
	//	{
	//		"TEXCOORD",
	//		0,
	//		DXGI_FORMAT_R32G32_FLOAT,
	//		0,
	//		12,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	}
	//};
	//this->vertex_shader = make_shared<VertexShader>(
	//	this->graphic->getDevice(),
	//	L"VertexShaderUV.hlsl",
	//	"main",
	//	"vs_5_0"
	//	);
	//this->input_layout = make_shared<InputLayout>(
	//	this->graphic->getDevice(),
	//	layout.data(),
	//	layout.size(),
	//	this->vertex_shader->getBlob()
	//	);

	//this->pixel_shader = make_shared<PixelShader>(
	//	this->graphic->getDevice(),
	//	L"PixelShaderUV.hlsl",
	//	"main",
	//	"ps_5_0"
	//	);

	//this->rasterizer_state = make_shared<RasterizerState>(
	//	this->graphic->getDevice(),
	//	D3D11_FILL_SOLID,
	//	D3D11_CULL_BACK
	//	);

	//this->sampler_state =
	//	make_shared<SamplerState>(
	//		this->graphic->getDevice()
	//		);

	//this->texture = make_shared<Texture>(
	//	this->graphic->getDevice(),
	//	L"Skeleton.png"
	//	);

	//this->blend_state = make_shared<BlendState>(
	//	this->graphic->getDevice()
	//	);

	//MVP mvp;
	//mvp.model = Mat::Identity;
	//mvp.view = Mat::Identity;
	//mvp.proj = Mat::Identity;
	//this->constant_buffer = make_shared<ConstantBuffer>(
	//	this->graphic->getDevice(),
	//	this->graphic->getContext(),
	//	mvp
	//	);
}

void Test2::renderUV()
{
	//this->graphic->renderBegin();
	ID3D11DeviceContext* device_context = this->context.getDeviceContext();
	//ID3D11RenderTargetView* rtv_array[] = {
		//this->swapchain_rtv.getInternalResource()
	//};
	IDXGISwapChain* swap_chain = this->swapchain.getInternalResource();
	ID3D11RenderTargetView*const* rtv_array = this->swapchain_rtv.getAddressOf();

	ID3D11DepthStencilView* dsv = this->dsv.getInternalResource();
	this->swapchain_rtv.clear(0.5f, 0.5f, 0.5f, 1.0f);
	this->dsv.clearDepth(1.0f);
	this->context.setViewport(0, 0, 400, 400);
	device_context->OMSetRenderTargets(
		1,
		rtv_array,
		dsv
	);
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//device_context->IASetInputLayout(
	//	
	//);

//	//IA
//	this->graphic->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	this->graphic->getContext()->IASetInputLayout(
//		this->input_layout->getComPtr().Get()
//	);
//	uint32 stride = this->vertex_uv_buffer->getStride();
//	uint32 offset = this->vertex_uv_buffer->getOffset();
//	this->graphic->getContext()->IASetVertexBuffers(
//		0,
//		1,
//		this->vertex_uv_buffer->getComPtr().GetAddressOf(),
//		&stride,
//		&offset
//	);
//	this->graphic->getContext()->IASetIndexBuffer(
//		this->index_buffer->getComPtr().Get(),
//		DXGI_FORMAT_R32_UINT,
//		0
//	);
//
//
//	//VS
//	this->graphic->getContext()->VSSetShader(
//		this->vertex_shader->getComPtr().Get(),
//		nullptr,
//		0
//	);
//	this->graphic->getContext()->VSSetConstantBuffers(
//		0,
//		1,
//		this->constant_buffer->getComPtr().GetAddressOf()
//	);
//
//	//RS
//	this->graphic->getContext()->RSSetState(
//		this->rasterizer_state->getComPtr().Get()
//	);
//
//	//PS
//	this->graphic->getContext()->PSSetShader(
//		this->pixel_shader->getComPtr().Get(),
//		nullptr,
//		0
//	);
//	this->graphic->getContext()->PSSetSamplers(
//		0,
//		1,
//		this->sampler_state->getComPtr().GetAddressOf()
//	);
//	this->graphic->getContext()->PSSetShaderResources(
//		0,
//		1,
//		this->texture->getComPtr().GetAddressOf()
//	);
//
//	//OM
//	this->graphic->getContext()->OMSetBlendState(
//		this->blend_state->getComPtr().Get(),
//		this->blend_state->getBlendFactor(),
//		this->blend_state->getSampleMask()
//	);
//
//	this->graphic->getContext()->DrawIndexed(
//		this->index_buffer->getCount(),
//		0,
//		0
//	);
//
	//this->graphic->renderEnd();
	swap_chain->Present(1, 0);
}

void Test2::update()
{
	MVP mvp;
	mvp.model = Mat::Identity;
	mvp.view = XMMatrixLookAtLH(
		vec3(0, 0, -5),
		vec3(0, 0, 1),
		vec3(0, 1, 0)
	);
	mvp.view = Mat::CreateRotationY(XMConvertToRadians(0)) * mvp.view;
	mvp.view = mvp.view.Transpose();
	mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(70),
		800 / 650,
		0.1f,
		1000.f
	);
	mvp.proj = mvp.proj.Transpose();

	//this->constant_buffer->update(mvp);

 
}

void Test2::geometrySkel()
{
	this->vertices_uv.resize(4);

	this->vertices_uv[0].pos = vec3(-1.f, 1.f, 0.f);
	this->vertices_uv[0].uv = vec2(0.f, 0.f);

	this->vertices_uv[1].pos = vec3(1.f, 1.f, 0.f);
	this->vertices_uv[1].uv = vec2(1.f, 0.f);

	this->vertices_uv[2].pos = vec3(1.f, -1.f, 0.f);
	this->vertices_uv[2].uv = vec2(1.f, 1.f);

	this->vertices_uv[3].pos = vec3(-1.f, -1.f, 0.f);
	this->vertices_uv[3].uv = vec2(0.f, 1.f);

	this->indices = { 0, 1, 2, 2, 3, 0 };
}
