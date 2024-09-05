#include "pch.h"
#include "Chunk.h"

#include "Buffer.h"
#include "Graphics.h"
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

void Chunk::setVIBuffer(
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
	this->render_flag = true;
}


void Chunk::readFile(string const& path)
{
}

void Chunk::updateFile() const
{
}
