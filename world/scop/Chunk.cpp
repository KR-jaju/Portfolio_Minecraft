#include "pch.h"
#include "Chunk.h"
#include "DepthMap.h"
#include "Buffer.h"
#include "Graphics.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "ConstantBuffer.h"
#include <fstream>
#include <sstream>

Chunk::Chunk()
{
	this->vertices_idx = 0;
	this->render_flag = false;
}

Chunk::~Chunk()
{
}



void Chunk::setRender(
	shared_ptr<Graphics> graphic, 
	shared_ptr<VertexShader> vertex_shader,
	bool shadow_map_flag
)
{
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();
	graphic->getContext()->IASetVertexBuffers(
		0,
		1,
		this->vertex_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	graphic->getContext()->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	if (shadow_map_flag == true) {
		ConstantBuffer pixel_cbuffer(
			graphic->getDevice(),
			graphic->getContext(),
			this->light
		);
		graphic->getContext()->PSSetConstantBuffers(
			1,
			1,
			pixel_cbuffer.getComPtr().GetAddressOf()
		);
		graphic->getContext()->PSSetShaderResources(
			1,
			1,
			this->depth_map->getShaderResourceView().GetAddressOf()
		);
	}
	graphic->getContext()->DrawIndexed(
		this->index_buffer->getCount(),
		0,
		0
	);
}

void Chunk::setDepthRender(
	shared_ptr<Graphics> graphic, 
	shared_ptr<VertexShader> vertex_shader
)
{
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();
	graphic->getContext()->IASetVertexBuffers(
		0,
		1,
		this->vertex_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	graphic->getContext()->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	ConstantBuffer vertex_cbuffer(
		graphic->getDevice(),
		graphic->getContext(),
		this->light
	);
	graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		vertex_cbuffer.getComPtr().GetAddressOf()
	);
	graphic->getContext()->OMSetRenderTargets(
		0,
		NULL,
		this->depth_map->getDepthStencilView().Get()
	);
	graphic->getContext()->ClearDepthStencilView(
		this->depth_map->getDepthStencilView().Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
	);
	graphic->getContext()->DrawIndexed(
		this->index_buffer->getCount(),
		0,
		0
	);
}

void Chunk::createVIBuffer(
	shared_ptr<Graphics> graphic,
	vector<VertexBlockUV> const& vertices, 
	vector<uint32> const& indices)
{
	this->vertex_buffer = make_shared<Buffer<VertexBlockUV>>(
		graphic->getDevice(),
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->index_buffer = make_shared<Buffer<uint32>>(
		graphic->getDevice(),
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->createDepthMap(graphic);
	this->render_flag = true;
}

void Chunk::reset()
{
	this->render_flag = false;
	this->vertices_idx = 0;
	this->max_h = -1;
}

void Chunk::setPos(Index2 const& c_pos)
{
	this->chunk_pos = c_pos;
	this->start_pos = vec3(c_pos.x + 0.5f, 0.5f, c_pos.y - 0.5f);
}

void Chunk::createDepthMap(shared_ptr<Graphics> graphic)
{
	this->depth_map = make_shared<DepthMap>(graphic->getDevice());
	float x = (this->chunk_pos.x * 2 + 16) * 0.5;
	float z = (this->chunk_pos.y * 2 - 16) * 0.5;
	float y = this->max_h + 1;
	this->light.view = XMMatrixLookToLH(
		vec3(x, y, z),
		vec3(0, -1, 0),
		vec3(0, 0, 1)
	);
	this->light.view = this->light.view.Transpose();
	this->light.proj = XMMatrixOrthographicLH(17, 17, 0.01, 1000);
	this->light.proj = this->light.proj.Transpose();
}

