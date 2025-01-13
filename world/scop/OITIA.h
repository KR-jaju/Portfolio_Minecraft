#pragma once

#include "MapUtils.h"

class DeferredGraphics;
class DeferredBuffer;
class InputLayout;
template<typename T> class Buffer;
class VertexShader;
class RasterizerState;
class PixelShader;
class SamplerState;

class OITIA
{
public:
	OITIA(DeferredGraphics* dgraphics, MapUtils* minfo);
	~OITIA();
	void setPipe();
	void render(
		ComPtr<ID3D11ShaderResourceView> terrain_srv,
		ComPtr<ID3D11ShaderResourceView> sky_srv
	);
	ComPtr<ID3D11RenderTargetView> getRTV();
	ComPtr<ID3D11ShaderResourceView> getSRV();

private:
	shared_ptr<Buffer<VertexDefer>> v_buff;
	shared_ptr<Buffer<uint32>> i_buff;

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	shared_ptr<DeferredBuffer> d_buff;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
};

