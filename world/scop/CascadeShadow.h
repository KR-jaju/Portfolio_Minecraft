#pragma once

class DeferredGraphics;
class DepthMap;
class ConstantBuffer;
class MapUtils;
class DeferredBuffer;
class RasterizerState;
class VertexShader;
class GeometryShader;
class PixelShader;
class InputLayout;
class TextureArray;
class StructuredBuffer;

struct FrustumVertex {
	vec4 front[4];
	vec4 back[4];
};

class CascadeShadow
{
public:
	CascadeShadow(
		DeferredGraphics* d_graphic, 
		UINT width, UINT height, MapUtils* m_info);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	MVP const& getMVP(
		Mat const& cam_view,
		Mat const& cam_proj,
		FrustumVertex const& frustum_vertices
	);
	void setPipe(shared_ptr<StructuredBuffer>& structured_buffer);
	void render();

private:
	void setDSVAndSRV(UINT width, UINT height);

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	MVP mvp;
	float width;
	float height;
	D3D11_VIEWPORT view_port;
	int split_cnt = 3;

private:
	ComPtr<ID3D11DepthStencilView> depth_view;
	ComPtr<ID3D11ShaderResourceView> t_arr_srv;
	shared_ptr<StructuredBuffer> structured_buffer;

private:
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<GeometryShader> geometry_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
};

