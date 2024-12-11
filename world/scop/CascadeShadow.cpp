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
	vec3 f_center = vec3(mid.x, mid.y, mid.z);
	Mat light_view_mat = XMMatrixLookAtLH(f_center - this->m_info->light_dir * len,
		f_center, vec3(0, 1, 0));

	vec3 max_orth = f_center + vec3(len);
	vec4 tt = XMVector4Transform(vec4(max_orth.x, max_orth.y, max_orth.z, 1),
		light_view_mat);
	max_orth = vec3(tt.x, tt.y, tt.z);
	vec3 min_orth = f_center - vec3(len);
	tt = XMVector4Transform(vec4(min_orth.x, min_orth.y, min_orth.z, 1),
		light_view_mat);
	min_orth = vec3(tt.x, tt.y, tt.z);

	float length_tmp = (max_orth - min_orth).Length();
	Mat light_orth_mat = XMMatrixOrthographicOffCenterLH(min_orth.x,
		max_orth.x, min_orth.y, max_orth.y, 0.1, 1000);
	Mat shadow_mat = light_view_mat * light_orth_mat;
	vec4 shadow_ori = vec4(0, 0, 0, 1);
	shadow_ori = XMVector4Transform(shadow_ori, shadow_mat);
	float s_w = shadow_ori.w;
	shadow_ori = shadow_ori * this->width * 0.5f;

	vec4 round_ori = vec4(round(shadow_ori.x), round(shadow_ori.y),
		round(shadow_ori.z), round(shadow_ori.w));
	vec4 round_offset = round_ori - shadow_ori;
	round_offset = round_offset * 2.0f / this->width;
	round_offset.z = 0.0f;
	round_offset.w = 0.0f;

	XMMATRIX shadowProj = light_orth_mat;
	XMVECTOR translation = shadowProj.r[3];
	translation = XMVectorAdd(translation, round_offset);
	shadowProj.r[3] = translation;
	light_orth_mat = shadowProj;

	this->mvp.view = light_view_mat.Transpose();
	this->mvp.proj = light_orth_mat.Transpose();
	this->cbuffer->update(this->mvp); // light shimmering이 발생하지 않는 이유
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


