#include "pch.h"
#include "CascadeShadow.h"
#include "DepthMap.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "ConstantBuffer.h"
#include "MapUtils.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "RasterizerState.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "StructuredBuffer.h"
#include "Chunk.h"

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
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"ShadowVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_shadow.data(),
		InputLayouts::layout_shadow.size(),
		this->vertex_shader->getBlob()
	);
	this->geometry_shader = make_shared<GeometryShader>(
		device,
		L"ShadowGS.hlsl",
		"main",
		"gs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"ShadowPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->setDSVAndSRV(width, height);
	this->view_port.TopLeftX = 0;
	this->view_port.TopLeftY = 0;
	this->view_port.Width = static_cast<float>(width);
	this->view_port.Height = static_cast<float>(height);
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
}

ComPtr<ID3D11ShaderResourceView> CascadeShadow::getSRV()
{
	return this->t_arr_srv;
}

MVP const& CascadeShadow::getMVP(
	Mat const& cam_view,
	Mat const& cam_proj,
	FrustumVertex const& frustum_vertices
)
{
	float x = 0;
	float y = 0;
	float z = 0;
	Mat inv_view = cam_view.Invert();
	vector<vec4> coord;
	coord.resize(8);
	vec4 mid = vec4(0, 0, 0, 0);
	for (int j = 0; j < 4; j++) {// world space로 절두체 좌표값 저장
		vec4 front = XMVector4Transform(frustum_vertices.front[j], inv_view);
		vec4 back = XMVector4Transform(frustum_vertices.back[j], inv_view);
		mid += front + back;
		coord[j] = front;
		coord[4 + j] = back;
	}
	mid /= 8; // 절두체의 중점(world space)

	float len = (mid - coord[4]).Length(); // 절두체 반지름
	float texel_per_unit = this->width / (len * 2.0f);

	Mat scalar =
		XMMatrixScaling(texel_per_unit, texel_per_unit, texel_per_unit);
	vec3 zero(0, 0, 0);
	vec3 up_dir(0, 1, 0);
	vec3 base_look_at = -this->m_info->light_dir;
	Mat look_at = XMMatrixLookAtLH(zero, base_look_at, up_dir);
	look_at = scalar * look_at;
	Mat inv_look = look_at.Invert();

	mid.w = 1;
	mid = XMVector4Transform(mid, look_at);
	mid.x = floor(mid.x);
	mid.y = floor(mid.y);
	mid = XMVector4Transform(mid, inv_look);
	vec3 center = vec3(mid.x, mid.y, mid.z);
	vec3 eye = center - this->m_info->light_dir * len;
	Mat light_look = XMMatrixLookAtLH(eye, center, up_dir);
	this->mvp.view = light_look.Transpose();

	this->mvp.proj = XMMatrixOrthographicOffCenterLH(
		-len, len, -len, len, 0.1, 1000);
	this->mvp.proj = this->mvp.proj.Transpose();
	return this->mvp;
}


void CascadeShadow::setPipe(shared_ptr<StructuredBuffer>& structured_buffer)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr, 0);
	context->GSSetShader(
		this->geometry_shader->getComPtr().Get(),
		nullptr, 0);
	context->GSSetShaderResources(0, 1,
		structured_buffer->GetSRV().GetAddressOf());
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(this->pixel_shader->getComPtr().Get(),
		nullptr, 0);
	context->ClearDepthStencilView(this->depth_view.Get(),
		D3D11_CLEAR_DEPTH, 1.0f, 0.f);
	context->OMSetRenderTargets(0, nullptr, this->depth_view.Get());
	context->RSSetViewports(1, &(this->view_port));
}

void CascadeShadow::render()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->render_flag == false)
				continue;
			this->m_info->chunks[i][j]->setShadowRender(
				this->d_graphic->getContext(),
				this->vertex_shader
			);
		}
	}
	context->GSSetShader(nullptr, nullptr, 0);
	context->GSSetShaderResources(0, 0, nullptr);
}

void CascadeShadow::setDSVAndSRV(UINT width, UINT height)
{
	ComPtr<ID3D11Texture2D> tex;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 3;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_SHADER_RESOURCE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MiscFlags = 0;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	HRESULT hr = device->CreateTexture2D(&desc, nullptr, tex.GetAddressOf());
	CHECK(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));
	dsv_desc.Format = DXGI_FORMAT_D32_FLOAT; // D32 (R32)인 경우
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsv_desc.Texture2DArray.FirstArraySlice = 0;
	dsv_desc.Texture2DArray.ArraySize = 3;
	dsv_desc.Texture2DArray.MipSlice = 0;
	hr = device->CreateDepthStencilView(tex.Get(), &dsv_desc,
		this->depth_view.GetAddressOf());
	CHECK(hr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(srv_desc));
	srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.ArraySize = 3;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	hr = device->CreateShaderResourceView(
		tex.Get(), &srv_desc, this->t_arr_srv.GetAddressOf());
	CHECK(hr);
}


