#include "pch.h"
#include "Test.h"
#include "Pipeline.h"
#include "VertexArrayObject.h"
#include "Graphics.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "SamplerState.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "ConstantBuffer.h"

Test::Test(HWND hwnd, UINT width, UINT height)
{
	this->graphic = make_shared<Graphics>(hwnd, width, height);
	this->vao = make_shared<VertexArrayObject>(
		this->graphic->getDevice(),
		this->graphic->getContext()
	);
	this->pipe = make_shared<Pipeline>(
		this->graphic->getContext()
	);
}

Test::~Test()
{
}

void Test::setDrawBox()
{
	this->geometryBox();
	this->vao->createIndexBuffer(this->indices);
	this->vao->createVertexBuffer(this->vertices);
	vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	shared_ptr<VertexShader> vertex_shader =
		make_shared<VertexShader>(
			this->graphic->getDevice()
		);
	vertex_shader->create(
		L"VertexShader.hlsl",
		"main",
		"vs_5_0"
	);
	this->vao->createInputLayout(layout, vertex_shader->getBlob());
	this->pipe->setVertexShader(vertex_shader);

	shared_ptr<RasterizerState> raster = 
		make_shared<RasterizerState>(
		this->graphic->getDevice()
	);
	raster->create(D3D11_FILL_WIREFRAME, D3D11_CULL_BACK);
	this->pipe->setRasterizerState(raster);

	shared_ptr<PixelShader> pixel = make_shared<PixelShader>(
		this->graphic->getDevice()
	);
	pixel->create(
		L"PixelShader.hlsl",
		"main",
		"ps_5_0"
	);
	this->pipe->setPixelShader(pixel);

	MVP mvp;
	mvp.model = Mat::Identity;
	mvp.view = XMMatrixLookAtLH(
		vec3(0, 0, -5),
		vec3(0, 0, 1),
		vec3(0, 1, 0)
	);
	mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(70),
		800.f / 650.f,
		0.01f,
		1000.f
	);
	constant_buffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext()
	);
	constant_buffer->create(mvp);
}

void Test::setDrawTexSkel()
{
	this->geometrySkel();
	this->graphic->setClearColor(0.5f, 0.5f, 0.5f, 1.f);
	this->vao->createIndexBuffer(this->indices);
	this->vao->createVertexBuffer(this->vertices_uv);
	vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	shared_ptr<VertexShader> vertex_shader;
	vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice()
	);
	vertex_shader->create(
		L"VertexShaderUV.hlsl",
		"main",
		"vs_5_0"
	);
	this->pipe->setVertexShader(vertex_shader);
	this->vao->createInputLayout(
		layout,
		vertex_shader->getBlob()
	);

	shared_ptr<PixelShader> pixel_shader;
	pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice()
	);
	pixel_shader->create(
		L"PixelShaderUV.hlsl",
		"main",
		"ps_5_0"
	);
	this->pipe->setPixelShader(pixel_shader);

	shared_ptr<RasterizerState> r_state;
	r_state = make_shared<RasterizerState>(
		this->graphic->getDevice()
	);
	r_state->create(
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->pipe->setRasterizerState(r_state);

	shared_ptr<SamplerState> sampler_state =
		make_shared<SamplerState>(
			this->graphic->getDevice()
		);
	sampler_state->create();
	this->pipe->setSamplerState(sampler_state);
	
	shared_ptr<Texture> texture = make_shared<Texture>(
		this->graphic->getDevice()
	);
	texture->create(
		L"Skeleton.png"
	);
	this->pipe->setTexture(texture);

	shared_ptr<BlendState> blend_state = make_shared<BlendState>(
		this->graphic->getDevice()
	);
	blend_state->create();
	this->pipe->setBlendState(blend_state);

	this->constant_buffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext()
	);
	MVP mvp;
	mvp.model = Mat::Identity;
	mvp.view = Mat::Identity;
	mvp.proj = Mat::Identity;
	constant_buffer->create(mvp);
}

void Test::geometryBox()
{
	// 윗면
	vertices.resize(24);

	const float scale = 1.0f;
	vertices[0].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
	vertices[1].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
	vertices[2].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
	vertices[3].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
	vertices[0].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
	vertices[1].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
	vertices[2].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
	vertices[3].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));

	// 아랫면
	vertices[4].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
	vertices[5].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
	vertices[6].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
	vertices[7].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
	vertices[4].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
	vertices[5].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
	vertices[6].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
	vertices[7].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));

	// 앞면
	vertices[8].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
	vertices[9].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
	vertices[10].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
	vertices[11].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
	vertices[8].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
	vertices[9].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
	vertices[10].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
	vertices[11].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));

	// 뒷면
	vertices[12].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
	vertices[13].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
	vertices[14].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
	vertices[15].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
	vertices[12].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
	vertices[13].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
	vertices[14].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
	vertices[15].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));

	// 왼쪽
	vertices[16].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
	vertices[17].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
	vertices[18].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
	vertices[19].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
	vertices[16].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
	vertices[17].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
	vertices[18].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
	vertices[19].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));

	// 오른쪽
	vertices[20].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
	vertices[21].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
	vertices[22].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
	vertices[23].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
	vertices[20].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
	vertices[21].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
	vertices[22].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
	vertices[23].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));

	indices = {
		0,  1,  2,  0,  2,  3,  // 윗면
		4,  5,  6,  4,  6,  7,  // 아랫면
		8,  9,  10, 8,  10, 11, // 앞면
		12, 13, 14, 12, 14, 15, // 뒷면
		16, 17, 18, 16, 18, 19, // 왼쪽
		20, 21, 22, 20, 22, 23  // 오른쪽
	};
}


void Test::render()
{
	this->graphic->renderBegin();

	this->vao->setInputAsembler();

	this->pipe->updatePipeline();
	this->pipe->drawIndexed(
		this->indices.size(),
		0,
		0
	);

	this->graphic->renderEnd();
}

void Test::renderUV()
{
	this->graphic->renderBegin();

	this->vao->setInputAsembler();

	this->pipe->updatePipeline();

	this->pipe->setPipeTexture(
		0,
		static_cast<uint32>(ShaderType::PIXEL),
		1
	);
	this->pipe->setPipeSamplerState(
		0,
		static_cast<uint32>(ShaderType::PIXEL),
		1
	);

	this->pipe->drawIndexed(
		this->indices.size(),
		0,
		0
	);

	this->graphic->renderEnd();
}

void Test::update()
{
	MVP mvp;
	mvp.model = Mat::Identity;
	mvp.view = XMMatrixLookAtLH(
		vec3(0, 0, -2),
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

	constant_buffer->update(mvp);
	this->pipe->setPipeConstantBuffer(
		0,
		static_cast<uint32>(ShaderType::VERTEX),
		this->constant_buffer
	);
}

void Test::geometrySkel()
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
