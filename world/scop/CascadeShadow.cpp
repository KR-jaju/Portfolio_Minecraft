#include "pch.h"
#include "CascadeShadow.h"
#include "DepthMap.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "ConstantBuffer.h"
#include "MapUtils.h"

CascadeShadow::CascadeShadow(
	DeferredGraphics* d_graphic, 
	UINT width, 
	UINT height,
	MapUtils* m_info
)
{
	this->width = width;
	this->height = height;
	this->d_graphic = d_graphic;
	this->m_info = m_info;
	this->depth_buffer = make_shared<DepthMap>(
		this->d_graphic->getDevice(),
		width, height);
	this->depth_buffer->setViewPort(width, height);
	MVP mvp;
	this->cbuffer = make_shared<ConstantBuffer>(
		this->d_graphic->getDevice(),
		this->d_graphic->getContext(),
		mvp
	);
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->d_buffer->setRTVsAndSRVs(
		this->d_graphic->getDevice(),
		width,
		height
	);
}

ComPtr<ID3D11ShaderResourceView> CascadeShadow::getSRV()
{
	return this->depth_buffer->getShaderResourceView();
}

ComPtr<ID3D11DepthStencilView> CascadeShadow::getDSV()
{
	return this->depth_buffer->getDepthStencilView();
}

void CascadeShadow::setFrustumVertices(
	vec3 const& coord, 
	int vertex_idx, 
	int frustum_idx)
{
	this->frustum_vertices[frustum_idx][vertex_idx][0] = coord.x;
	this->frustum_vertices[frustum_idx][vertex_idx][1] = coord.y;
	this->frustum_vertices[frustum_idx][vertex_idx][2] = coord.z;
}



void CascadeShadow::updateCBuffer(
	Mat const& cam_view,
	Mat const& cam_proj
)
{
	float x = 0;
	float y = 0;
	float z = 0;
	Mat inv_view = cam_view.Invert();
	vector<vec4> coord;
	coord.resize(8);
	vec4 mid = vec4(0, 0, 0, 0);
	for (int i = 0; i < 2; i++) { // world space로 절두체 좌표값 저장
		for (int j = 0; j < 4; j++) {
			x = this->frustum_vertices[i][j][0];
			y = this->frustum_vertices[i][j][1];
			z = this->frustum_vertices[i][j][2];
			vec4 pos = XMVector4Transform(vec4(x, y, z, 1), inv_view);
			mid += pos;
			coord[i * 4 + j] = pos;
		}
	}
	mid /= 8; // 절두체의 중점(world space)

	float len = (mid - coord[4]).Length();
	float texel_per_unit = this->width / (len * 2.0f);

	vec3 ld = this->m_info->light_dir;
	vec4 l_dir = vec4(ld.x, ld.y, ld.z, 1);
	Mat scalar =
		XMMatrixScaling(texel_per_unit, texel_per_unit, texel_per_unit);
	vec3 zero(0, 0, 0);
	vec3 up_dir(0, 1, 0);
	vec3 base_look_at = vec3(l_dir.x, l_dir.y, l_dir.z);
	Mat look_at = XMMatrixLookAtLH(zero, base_look_at, up_dir);
	look_at = scalar * look_at;
	Mat inv_look = look_at.Invert();
	mid.w = 1;
	mid = XMVector4Transform(mid, look_at);
	mid.x = floor(mid.x);
	mid.y = floor(mid.y);
	mid = XMVector4Transform(mid, inv_look);

	vec3 center = vec3(mid.x, mid.y, mid.z);
	vec3 eye = center - base_look_at * 2.0f * len;
	Mat light_look = XMMatrixLookAtLH(eye, center, up_dir);
	this->mvp.view = light_look.Transpose();

	this->mvp.proj = XMMatrixOrthographicOffCenterLH(
		-len, len, -len, len, 0.1, 1000);
	this->mvp.proj = this->mvp.proj.Transpose();
	this->cbuffer->update(this->mvp);
}

shared_ptr<ConstantBuffer> CascadeShadow::getCBuffer()
{
	return this->cbuffer;
}

MVP const& CascadeShadow::getMVP()
{
	return this->mvp;
}

D3D11_VIEWPORT CascadeShadow::getViewPort()
{
	return this->depth_buffer->getViewPort();
}

shared_ptr<DeferredBuffer> CascadeShadow::getDBuffer()
{
	return this->d_buffer;
}


