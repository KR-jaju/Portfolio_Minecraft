#pragma once

class DeferredGraphics;
class DeferredBuffer;
class MapUtils;
class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class ConstantBuffer;

class CaveShadow
{
public:
	CaveShadow(DeferredGraphics* d_graphic, MapUtils* m_info);
	void render(Mat const& cam_view, Mat const& cam_proj);
	ComPtr<ID3D11ShaderResourceView> getSRV();

private:
	void setPipe();

private:
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<ConstantBuffer> c_buff;
	MapUtils* m_info;

private:
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
};

