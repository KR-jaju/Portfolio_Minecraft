#pragma once

#include "InputLayouts.h"

class MapUtils;
class DeferredGraphics;
class DeferredBuffer;
class RasterizerState;
class VertexShader;
class PixelShader;
class InputLayout;
class BlendState;
class DepthMap;

class ShadowRender
{
public:
	ShadowRender(
		MapUtils* minfo,
		DeferredGraphics* dgraphic
	);
	~ShadowRender();
	void render(
		Mat const& view,
		Mat const& proj
	);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	ComPtr<ID3D11ShaderResourceView> getDepthSRV();

private:
	void setPipe();

private:
	MapUtils* m_info;
	InputLayouts layout;
	DeferredGraphics* d_graphic;
	shared_ptr<DepthMap> depth_map;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<BlendState> blend_state;
};

