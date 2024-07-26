#include "pch.h"
#include "Game.h"
#include <sstream>

Game::Game()
{
}

Game::~Game()
{
}

void Game::Update()
{
	MVP data;
	data.model = Mat::Identity;
	data.model = data.model.Transpose();
	data.view = XMMatrixLookAtLH(
		vec3(0, 0, -5),
		vec3(0, 0, 1),
		vec3(0, 1, 0)
	);
	data.view = Mat::CreateRotationY(XMConvertToRadians(-45)) * data.view;
	data.view = data.view.Transpose();
	data.proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(90.0f),
		800.f / 650.f,
		0.01f,
		1000.f
	);
	data.proj = data.proj.Transpose();
	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));
	
	_context->Map(
		_constantBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&subResource
	);
	memcpy(subResource.pData, &data, sizeof(data));
	_context->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	renderBegin();
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		
		_context->IASetVertexBuffers(
			0,
			1,
			_vBuffer.GetAddressOf(),
			&stride,
			&offset
		);
		_context->IASetIndexBuffer(
			_iBuffer.Get(),
			DXGI_FORMAT_R32_UINT,
			0
		);
		_context->IASetInputLayout(_inputLayout.Get());

		
		_context->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_context->VSSetConstantBuffers(0, 1, 
			_constantBuffer.GetAddressOf());

		_context->RSSetState(_rasterizerState.Get());

		_context->PSSetShader(_pixelShader.Get(), nullptr, 0);

		_context->DrawIndexed(_indices.size(), 0, 0);
	}
	renderEnd();
}

void Game::Init(HWND hWnd, int flag)
{
	_hwnd = hWnd;
	createDeviceAndSwapChain();
	createRenderTargetView();
	setViewPort();

	if (flag == 0)
		createGeoMetry();
	else if (flag == 1)
		createGeoMetry2();
	else
		createBox();
	createVS();
	createPS();
	createInputLayout();

	createRasterizerState();
	createConstantBuffer();
}

void Game::createDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferDesc.Width = 800;
	desc.BufferDesc.Height = 650;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = _hwnd;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_context.GetAddressOf()
	);
	CHECK(hr);
}

void Game::createRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void **)backBuffer.GetAddressOf()
	);
	CHECK(hr);
	
	hr = _device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		_renderTargetView.GetAddressOf()
	);
	CHECK(hr);
}

void Game::setViewPort()
{
	ZeroMemory(&_viewPort, sizeof(_viewPort));
	_viewPort.TopLeftX = 0.f;
	_viewPort.TopLeftY = 0.f;
	_viewPort.Width = static_cast<float>(800);
	_viewPort.Height = static_cast<float>(650);
	_viewPort.MaxDepth = 1.f;
	_viewPort.MinDepth = 0.f;

}

void Game::createGeoMetry()
{
	_vertices.resize(3);

	_vertices[0].pos = vec3(0.f, -0.5f, 0.f);
	_vertices[0].color = color(1.f, 0.f, 0.f, 1.f);

	_vertices[1].pos = vec3(0.f, 0.5f, 0.f);
	_vertices[1].color = color(0.f, 1.f, 0.f, 1.f);

	_vertices[2].pos = vec3(0.5f, -0.5f, 0.f);
	_vertices[2].color = color(0.f, 0.f, 1.f, 1.f);

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();
		HRESULT hr = _device->CreateBuffer(
			&desc,
			&data,
			_vBuffer.GetAddressOf()
		);
		CHECK(hr);
	}

	_indices = { 0, 1, 2 };

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = (uint32)sizeof(int) * _indices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		HRESULT hr = _device->CreateBuffer(
			&desc,
			&data,
			_iBuffer.GetAddressOf()
		);
		CHECK(hr);
	}
}

void Game::createBox()
{
	const float scale = 1.0f;

	// 윗면
	_vertices.resize(24);
	
	_vertices[0].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
	_vertices[1].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
	_vertices[2].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
	_vertices[3].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
	_vertices[0].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
	_vertices[1].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
	_vertices[2].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));
	_vertices[3].color = (vec4(1.0f, 0.0f, 0.0f, 1.f));

	// 아랫면
	_vertices[4].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
	_vertices[5].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
	_vertices[6].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
	_vertices[7].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
	_vertices[4].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
	_vertices[5].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
	_vertices[6].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));
	_vertices[7].color = (vec4(0.0f, 1.0f, 0.0f, 1.f));

	// 앞면
	_vertices[8].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
	_vertices[9].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
	_vertices[10].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
	_vertices[11].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
	_vertices[8].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
	_vertices[9].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
	_vertices[10].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));
	_vertices[11].color = (vec4(0.0f, 0.0f, 1.0f, 1.f));

	// 뒷면
	_vertices[12].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
	_vertices[13].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
	_vertices[14].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
	_vertices[15].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
	_vertices[12].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
	_vertices[13].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
	_vertices[14].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));
	_vertices[15].color = (vec4(0.0f, 1.0f, 1.0f, 1.f));

	// 왼쪽
	_vertices[16].pos = (vec3(-1.0f, -1.0f, 1.0f) * scale);
	_vertices[17].pos = (vec3(-1.0f, 1.0f, 1.0f) * scale);
	_vertices[18].pos = (vec3(-1.0f, 1.0f, -1.0f) * scale);
	_vertices[19].pos = (vec3(-1.0f, -1.0f, -1.0f) * scale);
	_vertices[16].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
	_vertices[17].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
	_vertices[18].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));
	_vertices[19].color = (vec4(1.0f, 1.0f, 0.0f, 1.f));

	// 오른쪽
	_vertices[20].pos = (vec3(1.0f, -1.0f, 1.0f) * scale);
	_vertices[21].pos = (vec3(1.0f, -1.0f, -1.0f) * scale);
	_vertices[22].pos = (vec3(1.0f, 1.0f, -1.0f) * scale);
	_vertices[23].pos = (vec3(1.0f, 1.0f, 1.0f) * scale);
	_vertices[20].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
	_vertices[21].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
	_vertices[22].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));
	_vertices[23].color = (vec4(1.0f, 0.0f, 1.0f, 1.f));

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();
		HRESULT hr = _device->CreateBuffer(
			&desc,
			&data,
			_vBuffer.GetAddressOf()
		);
		CHECK(hr);
	}

	_indices = {
		0,  1,  2,  0,  2,  3,  // 윗면
		4,  5,  6,  4,  6,  7,  // 아랫면
		8,  9,  10, 8,  10, 11, // 앞면
		12, 13, 14, 12, 14, 15, // 뒷면
		16, 17, 18, 16, 18, 19, // 왼쪽
		20, 21, 22, 20, 22, 23  // 오른쪽
	};

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = (uint32)sizeof(int) * _indices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		HRESULT hr = _device->CreateBuffer(
			&desc,
			&data,
			_iBuffer.GetAddressOf()
		);
		CHECK(hr);
	}
}




void Game::createGeoMetry2()
{
	string str;
	string t, x, y, z;
	Vertex ver;
	ifstream file("resources/resources/teapot2.obj");

	while (getline(file, str)) {
		stringstream ss(str);
		if (str[0] == 'v') {
			ss >> t >> x >> y >> z;
			ver.pos = vec3(stof(x), stof(y), stof(z));
			int flag = _vertices.size() % 3;
			if (flag)
				ver.color = color(1.f, 1.f, 1.f, 1.f);
			else
				ver.color = color(0.5f, 0.5f, 0.5f, 1.f);
			_vertices.push_back(ver);
		}
		else if (str[0] == 'f') {
			ss >> t >> x >> y >> z;
			_indices.push_back(stoi(x));
			_indices.push_back(stoi(y));
			_indices.push_back(stoi(z));
		}
	}
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();
		_device->CreateBuffer(&desc, &data, _vBuffer.GetAddressOf());
	}
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = (uint32)sizeof(int) * _indices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();
		_device->CreateBuffer(&desc, &data, _iBuffer.GetAddressOf());
	}
}


void Game::createVS()
{
	LoadShaderFromFile(
		L"VertexShader.hlsl",
		"main",
		"vs_5_0",
		_vsBlob
	);
	HRESULT hr =
		_device->CreateVertexShader(
			_vsBlob->GetBufferPointer(),
			_vsBlob->GetBufferSize(),
			nullptr,
			_vertexShader.GetAddressOf()
		);
	CHECK(hr);
}

void Game::createInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] = {
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

	const int32 count = sizeof(layout) /
		sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(
		layout,
		count,
		_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		_inputLayout.GetAddressOf()
	);
}

void Game::createPS()
{
	LoadShaderFromFile(
		L"PixelShader.hlsl",
		"main",
		"ps_5_0",
		_pxBlob
	);
	HRESULT hr =
		_device->CreatePixelShader(
			_pxBlob->GetBufferPointer(),
			_pxBlob->GetBufferSize(),
			nullptr,
			_pixelShader.GetAddressOf()
		);
	CHECK(hr);
}

void Game::createRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	//desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_BACK;

	HRESULT hr = _device->CreateRasterizerState(
		&desc,
		_rasterizerState.GetAddressOf()
	);
}

void Game::createConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(MVP);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = _device->CreateBuffer(
		&desc,
		nullptr,
		_constantBuffer.GetAddressOf()
	);

}

void Game::LoadShaderFromFile(
	const wstring& path, 
	const string& name, 
	const string& version, 
	ComPtr<ID3DBlob>& blob
)
{
	const uint32 compilerFlag = 
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compilerFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);
}


void Game::renderBegin()
{
	_context->OMSetRenderTargets(
		1,
		_renderTargetView.GetAddressOf(),
		nullptr
	);
	_context->ClearRenderTargetView(
		_renderTargetView.Get(),
		clearColor
	);
	_context->RSSetViewports(1, &_viewPort);
}

void Game::renderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}
