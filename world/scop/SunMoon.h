#pragma once

class DeferredGraphics;
class DeferredBuffer;
class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class Sun;
class Moon;

class SunMoon
{
public:
	SunMoon(
		DeferredGraphics* dgraphic, 
		UINT width,
		UINT height
	);
	void render(
		vec3 const& cam_pos, 
		Mat const& cam_view,
		Mat const& cam_proj
	);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	vec3 getLightPos();

private:
	void setPipe();

private:
	shared_ptr<Sun> sun;
	shared_ptr<Moon> moon;
	vec3 sun_pos;
	vec3 moon_pos;
private:
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
};

