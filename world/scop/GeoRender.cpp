#include "pch.h"
#include "GeoRender.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "MapUtils.h"
#include "RasterizerState.h"
#include "TextureArray.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "ConstantBuffer.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "Chunk.h"
#include "DepthMap.h"

GeoRender::GeoRender(
	MapUtils* minfo,
	DeferredGraphics* dgraphic
)
{
	this->m_info = minfo;
	this->d_graphic = dgraphic;
	this->d_buffer = make_shared<DeferredBuffer>(8);
	this->d_buffer->setRTVsAndSRVs(
		this->d_graphic->getDevice(),
		this->m_info->width,
		this->m_info->height
	);
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		//D3D11_FILL_WIREFRAME,
		D3D11_CULL_BACK
	);
	vector<wstring> path_color = {
		L"./textures/pbr/test_sample/grass_path_top.png",
		L"./textures/pbr/test_sample/grass_path_side.png",
		L"./textures/pbr/test_sample/packed_mud.png",
		L"./textures/pbr/oak_tree/oak_log_top.png",
		L"./textures/pbr/oak_tree/oak_log.png",
		L"./textures/pbr/oak_tree/oak_log_top.png",
		L"./textures/pbr/oak_tree/oak_leaves.png",
		L"./textures/pbr/oak_tree/oak_leaves.png",
		L"./textures/pbr/oak_tree/oak_leaves.png"
	};
	path_color = {
		L"./textures/pbr/test_sample/packed_mud.png",
		L"./textures/pbr/test_sample/packed_mud.png",
		L"./textures/pbr/test_sample/packed_mud.png",
		L"./textures/pbr/oak_tree/oak_log_top.png",
		L"./textures/pbr/oak_tree/oak_log.png",
		L"./textures/pbr/oak_tree/oak_log_top.png",
		L"./textures/pbr/oak_tree/oak_leaves.png",
		L"./textures/pbr/oak_tree/oak_leaves.png",
		L"./textures/pbr/oak_tree/oak_leaves.png"
	};
	this->texture_array_color = 
		make_shared<TextureArray>(
			device,
			context,
			path_color,
			0, true // temp
		);
	vector<wstring> path_normal = {
		L"./textures/pbr/test_sample/grass_path_top_n.png",
		L"./textures/pbr/test_sample/grass_path_side_n.png",
		L"./textures/pbr/test_sample/packed_mud_n.png",
		L"./textures/pbr/oak_tree/oak_log_top_n.png",
		L"./textures/pbr/oak_tree/oak_log_n.png",
		L"./textures/pbr/oak_tree/oak_log_top_n.png",
		L"./textures/pbr/oak_tree/oak_leaves_n.png",
		L"./textures/pbr/oak_tree/oak_leaves_n.png",
		L"./textures/pbr/oak_tree/oak_leaves_n.png"
	};
	path_normal = {
		L"./textures/pbr/test_sample/packed_mud_n.png",
		L"./textures/pbr/test_sample/packed_mud_n.png",
		L"./textures/pbr/test_sample/packed_mud_n.png",
		L"./textures/pbr/oak_tree/oak_log_top_n.png",
		L"./textures/pbr/oak_tree/oak_log_n.png",
		L"./textures/pbr/oak_tree/oak_log_top_n.png",
		L"./textures/pbr/oak_tree/oak_leaves_n.png",
		L"./textures/pbr/oak_tree/oak_leaves_n.png",
		L"./textures/pbr/oak_tree/oak_leaves_n.png"
	};
	this->texture_array_normal = 
		make_shared<TextureArray>(
			device,
			context,
			path_normal,
			0
		);
	  
	vector<wstring> path_s = {
		L"./textures/pbr/test_sample/grass_path_top_s.png",
		L"./textures/pbr/test_sample/grass_path_side_s.png",
		L"./textures/pbr/test_sample/packed_mud_s.png",
		L"./textures/pbr/oak_tree/oak_log_top_s.png",
		L"./textures/pbr/oak_tree/oak_log_s.png",
		L"./textures/pbr/oak_tree/oak_log_top_s.png",
		L"./textures/pbr/oak_tree/oak_leaves_s.png",
		L"./textures/pbr/oak_tree/oak_leaves_s.png",
		L"./textures/pbr/oak_tree/oak_leaves_s.png"
	};
	path_s = {
		L"./textures/pbr/test_sample/packed_mud_s.png",
		L"./textures/pbr/test_sample/packed_mud_s.png",
		L"./textures/pbr/test_sample/packed_mud_s.png",
		L"./textures/pbr/oak_tree/oak_log_top_s.png",
		L"./textures/pbr/oak_tree/oak_log_s.png",
		L"./textures/pbr/oak_tree/oak_log_top_s.png",
		L"./textures/pbr/oak_tree/oak_leaves_s.png",
		L"./textures/pbr/oak_tree/oak_leaves_s.png",
		L"./textures/pbr/oak_tree/oak_leaves_s.png"
	};
	this->texture_array_s = 
		make_shared<TextureArray>(
			device,
			context,
			path_s,
			0
		);
	// texture manager: texture array setting

	this->linear_state = make_shared<SamplerState>(device);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"GeometryVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"GeometryPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_ipntt.data(),
		InputLayouts::layout_ipntt.size(),
		this->vertex_shader->getBlob()
	);
	this->hull_shader = make_shared<HullShader>(
		device,
		L"GeometryHS.hlsl",
		"main",
		"hs_5_0"
	);
	this->domain_shader = make_shared<DomainShader>(
		device,
		L"GeometryDS.hlsl",
		"main",
		"ds_5_0"
	);
	MVP mvp;
	this->mvp_cbuffer = make_shared<ConstantBuffer>(
		device,
		context,
		mvp
	);
	CamPos cam;
	this->cam_pos_cbuffer = make_shared<ConstantBuffer>(
		device,
		context,
		cam
	);
	vec4 eye;
	this->eye_pos_cbuffer = make_shared<ConstantBuffer>(
		device,
		context,
		eye
	);
	if (this->parallax_flag) {
		this->parallax_mapping = make_shared<ParallaxMapping>(
			this->d_graphic,
			this->m_info->width,
			this->m_info->height
		);
	}
	this->depth_map = make_shared<DepthMap>(
		this->d_graphic->getDevice(),
		this->m_info->width, this->m_info->height
	);
}

GeoRender::~GeoRender()
{
}

void GeoRender::render(
	Mat const& view, 
	Mat const& proj,
	vec3 const& cam_pos
)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_graphic->renderBegin(this->d_buffer.get(), 
		this->depth_map->getDepthStencilView());
	this->setPipe();
	this->setConstantBuffer(view, proj, cam_pos);
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->render_flag == false)
				continue;
			this->m_info->chunks[i][j]->setGeoRender(
				this->d_graphic->getContext()
			);
		}
	}
	context->HSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
	if (this->parallax_flag)
		this->parallaxRender(cam_pos);
}

ComPtr<ID3D11ShaderResourceView> GeoRender::getSRV(RTVIndex idx)
{
	int index = static_cast<int>(idx);
	if (this->parallax_flag)
		return this->parallax_mapping->getSRV(idx);
	return this->d_buffer->getSRV(index);
}

ComPtr<ID3D11ShaderResourceView> GeoRender::getNPSRV(RTVIndex idx)
{
	int index = static_cast<int>(idx);
	return this->d_buffer->getSRV(index);
}

ComPtr<ID3D11ShaderResourceView> GeoRender::getDepthSRV()
{
	return this->depth_map->getShaderResourceView();
}

ComPtr<ID3D11DepthStencilView> GeoRender::getDSV()
{
	return this->depth_map->getDepthStencilView();
}

ComPtr<ID3D11RenderTargetView> GeoRender::getRTV(RTVIndex idx)
{
	int index = static_cast<int>(idx);
	if (this->parallax_flag)
		return this->parallax_mapping->getRTV(idx);
	return this->d_buffer->getRTV(index);
}

void GeoRender::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->PSSetSamplers(
		0,
		1,
		this->linear_state->getComPtr().GetAddressOf()
	);
	context->PSSetShaderResources(
		0,
		1,
		this->texture_array_color->getComPtr().GetAddressOf()
	);
	context->PSSetShaderResources(
		1,
		1,
		this->texture_array_s->getComPtr().GetAddressOf()
	);
	context->PSSetShaderResources(
		2,
		1,
		this->texture_array_normal->getComPtr().GetAddressOf()
	);
	context->HSSetShader(
		this->hull_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->DSSetShader(
		this->domain_shader->getComPtr().Get(),
		nullptr,
		0
	);

	// 실제 vertex위치 옮기는 경우
	context->DSSetShaderResources(0, 1,
		this->texture_array_normal->getComPtr().GetAddressOf());
}

void GeoRender::setConstantBuffer(
	Mat const& view, 
	Mat const& proj, 
	vec3 const& cam_pos
)
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();

	// set hs constant
	vec4 eye;
	eye.x = cam_pos.x;
	eye.y = cam_pos.y;
	eye.z = cam_pos.z;
	eye.w = 1;
	this->eye_pos_cbuffer->update(eye);
	context->HSSetConstantBuffers(0, 1,
		this->eye_pos_cbuffer->getComPtr().GetAddressOf());
	
	// set ds constant
	MVP mvp;  
	mvp.view = view.Transpose();
	mvp.proj = proj.Transpose();
	this->mvp_cbuffer->update(mvp);
	context->DSSetConstantBuffers(0, 1,
		this->mvp_cbuffer->getComPtr().GetAddressOf());
	context->DSSetConstantBuffers(1, 1,
		this->eye_pos_cbuffer->getComPtr().GetAddressOf());

}

void GeoRender::parallaxRender(vec3 const& cam_pos)
{
	this->parallax_mapping->setRTV();
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->PSSetShaderResources(0, 1,
		this->texture_array_color->getComPtr().GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->texture_array_normal->getComPtr().GetAddressOf());
	context->PSSetShaderResources(2, 1,
		this->texture_array_s->getComPtr().GetAddressOf());
	context->PSSetShaderResources(3, 1,
		this->d_buffer->getSRV(5).GetAddressOf());
	context->PSSetShaderResources(4, 1,
		this->d_buffer->getSRV(1).GetAddressOf());
	context->PSSetShaderResources(5, 1,
		this->d_buffer->getSRV(6).GetAddressOf());
	context->PSSetShaderResources(6, 1,
		this->d_buffer->getSRV(7).GetAddressOf());
	this->parallax_mapping->render(cam_pos);
}
