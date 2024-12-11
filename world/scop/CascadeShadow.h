#pragma once

class DeferredGraphics;
class DepthMap;
class ConstantBuffer;
class MapUtils;
class DeferredBuffer;

class CascadeShadow
{
public:
	CascadeShadow(
		DeferredGraphics* d_graphic, 
		UINT width, UINT height, MapUtils* m_info);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	ComPtr<ID3D11DepthStencilView> getDSV();
	void setFrustumVertices(
		vec3 const& coord,
		int vertex_idx,
		int frustum_idx
	);
	void updateCBuffer(
		Mat const& cam_view, 
		Mat const& cam_proj
	);
	shared_ptr<ConstantBuffer> getCBuffer();
	MVP const& getMVP();
	D3D11_VIEWPORT getViewPort();

public: // test
	shared_ptr<DeferredBuffer> getDBuffer();

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	shared_ptr<DepthMap> depth_buffer;
	float frustum_vertices[2][4][3];
	shared_ptr<ConstantBuffer> cbuffer;
	MVP mvp;
	float width;
	float height;

private: // test
	shared_ptr<DeferredBuffer> d_buffer;
};

