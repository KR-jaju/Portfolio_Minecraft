#include "pch.h"
#include "Test.h"
#include "TimeSystem.h"
#include "InputSystem.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceSystem.h"
#include "SceneSystem.h"
#include "Transform.h"
#include "RenderSystem.h"
//Test::Test(HWND hwnd, UINT width, UINT height)
//{
//	this->graphic = make_shared<Graphics>(hwnd, width, height);
//	this->time_system = make_shared<TimeSystem>();
//	this->input_system = make_shared<InputSystem>(hwnd);
//	this->resource_system = make_shared<ResourceSystem>(this->graphic->getDevice());
//	this->resource_system->Init();
//	this->scene_system = make_shared<SceneSystem>(this->graphic, this->resource_system);
//	this->scene_system->Init();
//	this->render_system = make_shared<RenderSystem>(this->graphic->getDevice(), this->graphic->getContext(), this->scene_system);
//	this->render_system->Init();
//}
//
//Test::~Test()
//{
//
//}
//
//void Test::setDrawBox()
//{
//	this->geometryBox();
//	this->graphic->setClearColor(0.5f, 0.5f, 0.5f, 1.f);
//	this->index_buffer = make_shared<Buffer<uint32>>(
//		this->graphic->getDevice(),
//		this->indices.data(),
//		this->indices.size(),
//		D3D11_BIND_INDEX_BUFFER
//	);
//	this->vertex_buffer = make_shared<Buffer<Vertex>>(
//		this->graphic->getDevice(),
//		this->vertices.data(),
//		static_cast<uint32>(this->vertices.size()),
//		D3D11_BIND_VERTEX_BUFFER
//	);
//	vector<D3D11_INPUT_ELEMENT_DESC> layout = {
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
//	};
//	this->vertex_shader =
//		make_shared<VertexShader>(
//			this->graphic->getDevice(),
//			L"VertexShader.hlsl",
//			"main",
//			"vs_5_0"
//		);
//	this->input_layout = make_shared<InputLayout>(
//		this->graphic->getDevice(),
//		layout.data(),
//		layout.size(),
//		this->vertex_shader->getBlob()
//	);
//
//	this->rasterizer_state = 
//		make_shared<RasterizerState>(
//		this->graphic->getDevice(),
//			D3D11_FILL_SOLID,
//			D3D11_CULL_BACK
//	);
//
//	this->pixel_shader = make_shared<PixelShader>(
//		this->graphic->getDevice(),
//		L"PixelShader.hlsl",
//		"main",
//		"ps_5_0"
//	);
//	MVP mvp;
//	mvp.model = Mat::Identity;
//	mvp.view = Mat::Identity;
//	mvp.proj = Mat::Identity;
//	this->constant_buffer = make_shared<ConstantBuffer>(
//		this->graphic->getDevice(),
//		this->graphic->getContext(),
//		mvp
//	);
//}
//
//void Test::geometryBox()
//{
//	// 윗면
	//vertices.resize(24);
//
//	const float scale = 1.0f;
//	vertices[0].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
//	vertices[1].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
//	vertices[2].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
//	vertices[3].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
//	vertices[0].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
//	vertices[1].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
//	vertices[2].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
//	vertices[3].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
//
//	// 아랫면
//	vertices[4].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
//	vertices[5].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
//	vertices[6].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
//	vertices[7].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
//	vertices[4].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
//	vertices[5].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
//	vertices[6].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
//	vertices[7].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
//
//	// 앞면
//	vertices[8].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
//	vertices[9].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
//	vertices[10].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
//	vertices[11].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
//	vertices[8].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
//	vertices[9].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
//	vertices[10].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
//	vertices[11].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
//
//	// 뒷면
//	vertices[12].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
//	vertices[13].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
//	vertices[14].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
//	vertices[15].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
//	vertices[12].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
//	vertices[13].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
//	vertices[14].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
//	vertices[15].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
//
//	// 왼쪽
//	vertices[16].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
//	vertices[17].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
//	vertices[18].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
//	vertices[19].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
//	vertices[16].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
//	vertices[17].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
//	vertices[18].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
//	vertices[19].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
//
//	// 오른쪽
//	vertices[20].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
//	vertices[21].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
//	vertices[22].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
//	vertices[23].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
//	vertices[20].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
//	vertices[21].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
//	vertices[22].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
//	vertices[23].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
//
//	indices = {
//		0,  1,  2,  0,  2,  3,  // 윗면
//		4,  5,  6,  4,  6,  7,  // 아랫면
//		8,  9,  10, 8,  10, 11, // 앞면
//		12, 13, 14, 12, 14, 15, // 뒷면
//		16, 17, 18, 16, 18, 19, // 왼쪽
//		20, 21, 22, 20, 22, 23  // 오른쪽
//	};
//}
//
//
//void Test::render()
//{
//	this->graphic->renderBegin();
//	this->graphic->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	this->graphic->getContext()->IASetInputLayout(
//		this->input_layout->getComPtr().Get()
//	);
//	uint32 stride = this->vertex_buffer->getStride();
//	uint32 offset = this->vertex_buffer->getOffset();
//	this->graphic->getContext()->IASetVertexBuffers(
//		0,
//		1,
//		this->vertex_buffer->getComPtr().GetAddressOf(),
//		&stride,
//		&offset
//	);
//	this->graphic->getContext()->IASetIndexBuffer(
//		this->index_buffer->getComPtr().Get(),
//		DXGI_FORMAT_R32_UINT,
//		0
//	);
//
//	this->graphic->getContext()->VSSetShader(
//		this->vertex_shader->getComPtr().Get(),
//		nullptr,
//		0
//	);
//
//	this->graphic->getContext()->VSSetConstantBuffers(
//		0,
//		1,
//		this->constant_buffer->getComPtr().GetAddressOf()
//	);
//
//	this->graphic->getContext()->RSSetState(
//		this->rasterizer_state->getComPtr().Get()
//	);
//
//	this->graphic->getContext()->PSSetShader(
//		this->pixel_shader->getComPtr().Get(),
//		nullptr,
//		0
//	);
//
//
//	this->graphic->getContext()->DrawIndexed(
//		this->index_buffer->getCount(),
//		0,
//		0
//	);
//
//	this->graphic->renderEnd();
//}
//
//void Test::update()
//{
//	this->camera->Update();
//	this->input_system->Update();
//	this->time_system->Update();
//	this->camera_move->Update();
//	MVP constant_mvp;
//	constant_mvp.proj = this->camera->GetCamera()->GetProjectionMatrix().Transpose();
//	constant_mvp.view = this->camera->GetCamera()->GetViewMatrix().Transpose();
//	this->constant_buffer->update(constant_mvp);
//	
//}
//
//void Test::setContext()
//{
//}
//
//void Test::setCamera()
//{
//	this->camera = make_shared<GameObject>();
//	this->camera->GetOrAddTransform();
//	this->camera->AddComponent(make_shared<Camera>(800.f, 600.f));
//	this->camera->GetTransform()->SetPosition(vec3(0.f, 0.f, -5.f));
//	this->camera_move = make_shared<CameraMove>(input_system, time_system, camera);
//}

Test::Test(HWND hwnd, UINT width, UINT height)
{
	this->hwnd = hwnd;
	this->graphic = make_shared<Graphics>(this->hwnd, width, height);
	this->input_system = make_shared<InputSystem>(this->hwnd);
	this->time_system = make_shared<TimeSystem>();
	this->resource_system = make_shared<ResourceSystem>(this->graphic->getDevice());
	this->resource_system->Init();
	this->scene_system = make_shared<SceneSystem>(this->graphic, 
		this->resource_system,
		this->input_system,
		this->time_system
	);
	this->scene_system->Init();
	this->render_system = make_shared<RenderSystem>(this->graphic->getDevice(), this->graphic->getContext(), this->scene_system);
	this->render_system->Init();
	this->scene_system->LoadScene(L"Minecraft");
}

Test::~Test()
{
}


void Test::Update()
{
	this->time_system->Update();
	this->input_system->Update();
	this->scene_system->Update();
}

void Test::Render()
{
		this->render_system->Update(this->graphic);
}
