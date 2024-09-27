#pragma once

#include "InputLayouts.h"

class Graphics;
class RasterizerState;
class BlendState;
class TextureArray;
class SamplerState;
class VertexShader;
class PixelShader;
class InputLayout;
class MapUtils;

class RenderSystem
{
public:
	RenderSystem(MapUtils* minfo);
	~RenderSystem();
	void setGraphic(shared_ptr<Graphics> graphic);
	void setRender();
	void setDepthRender();
	void setDepthPipeLine();
	void setPipeLine();
	void Render(
		Mat const& cam_view,
		Mat const& cam_proj,
		vec3 const& cam_pos
	);
	void depthRender();

private:
	MapUtils* m_info;
	InputLayouts layout;
	int render_flag = 0;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<RasterizerState> depth_rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<SamplerState> depth_sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> depth_vertex_shader;
	shared_ptr<PixelShader> depth_pixel_shader;
	shared_ptr<InputLayout> depth_input_layout;
};

