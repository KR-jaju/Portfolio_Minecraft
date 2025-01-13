#pragma once

class DeferredGraphics;
class DeferredBuffer;
class MapUtils;
class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class ConstantBuffer;
class SamplerState;

class Transparent
{
public:
	Transparent(DeferredGraphics* d_graphic, MapUtils* minfo);
	~Transparent();
	void setPipe();
	void render(
		Mat const& cam_view, 
		Mat const& cam_proj,
		ComPtr<ID3D11ShaderResourceView> depth_srv
	);
	ComPtr<ID3D11ShaderResourceView> getAccum();
	ComPtr<ID3D11ShaderResourceView> getReveal();

private:
	ComPtr<ID3D11BlendState> blend_state;
	ComPtr<ID3D11DepthStencilState> prev_ds_state;
	UINT prev_ref;
	ComPtr<ID3D11DepthStencilState> ds_state;
	float clear_accum[4] = { 0.f, 0.f, 0.f, 0.f };
	float clear_reveal[4] = { 1.f, 1.f, 1.f, 1.f };

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<SamplerState> sampler_state;
};

