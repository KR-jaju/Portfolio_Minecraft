#pragma once

class MapUtils;
class DeferredGraphics;
class BlendState; // 바뀔 수 있음 나중에(ex OIT)
class TextureArray;
class SamplerState;
class VertexShader;
class PixelShader;
class InputLayout;
class RasterizerState;
class DeferredBuffer;
class HullShader;
class DomainShader;
class ConstantBuffer;
class Texture;

enum class RTVIndex
{
	color,
	vs_normal, // vs = view space
	vs_position,
	w_position,
	w_normal,
	metallic,
	roughness
};

class GeoRender
{
public:
	GeoRender(
		MapUtils* minfo,
		DeferredGraphics* dgraphic
	);
	~GeoRender();
	void render(
		Mat const& view,
		Mat const& proj,
		vec3 const& cam_pos
	);
	ComPtr<ID3D11ShaderResourceView> getSRV(RTVIndex idx);

private:
	void setPipe();
	void setConstantBuffer(
		Mat const& view,
		Mat const& proj,
		vec3 const& cam_pos
	);

private:
	MapUtils* m_info;
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> linear_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<BlendState> blend_state;
	shared_ptr<HullShader> hull_shader;
	shared_ptr<DomainShader> domain_shader;

private:
	shared_ptr<TextureArray> texture_array_color;
	shared_ptr<TextureArray> texture_array_normal;
	shared_ptr<TextureArray> texture_array_s;

private:
	shared_ptr<ConstantBuffer> mvp_cbuffer;
	shared_ptr<ConstantBuffer> cam_pos_cbuffer;
	shared_ptr<ConstantBuffer> eye_pos_cbuffer;
	shared_ptr<ConstantBuffer> view_cbuffer;
};

