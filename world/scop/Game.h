#pragma once
class Game
{
public:
	Game();
	~Game();
	void Update();
	void Render();
	void Init(HWND hWnd, int flag);
	void createGeoMetry2();

private:
	void renderBegin();
	void renderEnd();

	void createDeviceAndSwapChain();
	void createRenderTargetView();
	void setViewPort();

	void createGeoMetry();
	void createBox();
	void createVS();
	void createInputLayout();
	void createPS();

	void createRasterizerState();

	void createConstantBuffer();

private:
	void LoadShaderFromFile(const wstring& path,
		const string& name, const string& version,
		ComPtr<ID3DBlob>& blob);

private:
	HWND _hwnd;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _context;
	ComPtr<IDXGISwapChain> _swapChain;

	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	
	D3D11_VIEWPORT _viewPort;
	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vBuffer;
	vector<int> _indices;
	ComPtr<ID3D11Buffer> _iBuffer;

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3DBlob> _vsBlob;

	ComPtr<ID3D11InputLayout> _inputLayout;

	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3DBlob> _pxBlob;

	ComPtr<ID3D11RasterizerState> _rasterizerState;

	ComPtr<ID3D11Buffer> _constantBuffer;
};
