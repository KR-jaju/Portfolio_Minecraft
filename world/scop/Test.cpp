#include "pch.h"
#include "Test.h"

Test::Test(HWND hwnd, UINT width, UINT height)
{
	this->graphic = make_shared<Graphics>(hwnd, width, height);
}

Test::~Test()
{
}

void Test::setDrawBox()
{
	this->geometryBox();
	this->index_buffer = make_shared<Buffer<uint32>>(
		this->graphic->getDevice(),
		this->indices.data(),
		this->indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->vertex_buffer = make_shared<Buffer<Vertex>>(
		this->graphic->getDevice(),
		this->vertices.data(),
		static_cast<uint32>(this->vertices.size()),
		D3D11_BIND_VERTEX_BUFFER
	);
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
	//		"COLOR",
	//		0,
	//		DXGI_FORMAT_R32G32B32A32_FLOAT,
	//		0,
	//		12,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	}
	//};
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
	this->vertex_shader =
		make_shared<VertexShader>(
			this->graphic->getDevice(),
			L"VertexShader.hlsl",
			"main",
			"vs_5_0"
		);
	this->input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout.data(),
		layout.size(),
		this->vertex_shader->getBlob()
	);

	this->rasterizer_state = 
		make_shared<RasterizerState>(
		this->graphic->getDevice(),
			D3D11_FILL_SOLID,
			D3D11_CULL_BACK
	);

	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"PixelShader.hlsl",
		"main",
		"ps_5_0"
	);

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
	this->constant_buffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		mvp
	);
}

void Test::setDrawTexSkel()
{
	this->geometrySkel();
	this->graphic->setClearColor(0.5f, 0.5f, 0.5f, 1.f);
	this->index_buffer = make_shared<Buffer<uint32>>(
		this->graphic->getDevice(),
		this->indices.data(),
		(uint32)(this->indices.size()),
		D3D11_BIND_INDEX_BUFFER
	);
	this->vertex_uv_buffer = make_shared<Buffer<VertexUV>>(
		this->graphic->getDevice(),
		this->vertices_uv.data(),
		(uint32)(this->vertices_uv.size()),
		D3D11_BIND_VERTEX_BUFFER
	);
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
	this->vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"VertexShaderUV.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout.data(),
		layout.size(),
		this->vertex_shader->getBlob()
	);

	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"PixelShaderUV.hlsl",
		"main",
		"ps_5_0"
	);

	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);

	this->sampler_state =
		make_shared<SamplerState>(
			this->graphic->getDevice()
		);
	
	this->texture = make_shared<Texture>(
		this->graphic->getDevice(),
		L"Skeleton.png"
	);

	this->blend_state = make_shared<BlendState>(
		this->graphic->getDevice()
	);

	MVP mvp;
	mvp.model = Mat::Identity;
	mvp.view = Mat::Identity;
	mvp.proj = Mat::Identity;
	this->constant_buffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		mvp
	);
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

	this->graphic->getContext()->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();
	this->graphic->getContext()->IASetVertexBuffers(
		0,
		1,
		this->vertex_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	this->graphic->getContext()->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);

	this->graphic->getContext()->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);

	this->graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		this->constant_buffer->getComPtr().GetAddressOf()
	);

	this->graphic->getContext()->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);

	this->graphic->getContext()->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);


	this->graphic->getContext()->DrawIndexed(
		this->index_buffer->getCount(),
		0,
		0
	);

	this->graphic->renderEnd();
}

void Test::renderUV()
{
	this->graphic->renderBegin();

	//IA
	this->graphic->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->graphic->getContext()->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	uint32 stride = this->vertex_uv_buffer->getStride();
	uint32 offset = this->vertex_uv_buffer->getOffset();
	this->graphic->getContext()->IASetVertexBuffers(
		0,
		1,
		this->vertex_uv_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	this->graphic->getContext()->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);


	//VS
	this->graphic->getContext()->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	this->graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		this->constant_buffer->getComPtr().GetAddressOf()
	);

	//RS
	this->graphic->getContext()->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);

	//PS
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
	this->graphic->getContext()->PSSetShaderResources(
		0,
		1,
		this->texture->getComPtr().GetAddressOf()
	);

	//OM
	this->graphic->getContext()->OMSetBlendState(
		this->blend_state->getComPtr().Get(),
		this->blend_state->getBlendFactor(),
		this->blend_state->getSampleMask()
	);

	this->graphic->getContext()->DrawIndexed(
		this->index_buffer->getCount(),
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

	this->constant_buffer->update(mvp);
}

void Test::setContext()
{
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
