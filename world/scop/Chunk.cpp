#include "pch.h"
#include "Chunk.h"

#include "Buffer.h"
#include "Graphics.h"
#include <fstream>
#include <sstream>

Chunk::Chunk()
{
	fill(
		this->chunk,
		this->chunk + 16 * 256 * 16,
		0
	);
	fill(&this->height_map[0][0], &this->height_map[0][0] + 16 * 16, 0);
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

uint32 Chunk::getVerticesIdx() const
{
	return this->vertices_idx;
}

vec3 Chunk::getChunkPos() const
{
	return this->chunk_pos;
}

void Chunk::setVerticesIdx(uint32 idx)
{
	this->vertices_idx = idx;
}

void Chunk::addBlock(Index3 idx3, int16 type)
{
	this->chunk[idx3.x + idx3.y * 16 * 16 + idx3.z * 16] = type;
	this->block_cnt++;
}

void Chunk::addBlock(int x, int y, int z, int16 type)
{
	this->chunk[x + y * 16 * 16 + z * 16] = type;
	this->block_cnt++;
}

void Chunk::deleteBlock(vector<Index3> const& block_arr) // terrain
{
	int jz = 16;
	int jy = 16 * 16;
	for (int i = 0; i < block_arr.size(); i++) {
		this->chunk[block_arr[i].x + 
			block_arr[i].y * jy + block_arr[i].z * jz] = 0;
	}
}

int Chunk::getBlock(int x, int y, int z) const
{
	return this->chunk[x + y * 16 * 16 + z * 16];
}

int Chunk::getBlock(Index3 const& idx3) const
{
	return this->chunk[idx3.x + idx3.y * 16 * 16 + idx3.z * 16];
}

void Chunk::setStartPos(float x, float y, float z)
{
	this->start_pos = vec3(x, y, z) + vec3(0.5f, 0.5f, -0.5f);
	this->chunk_pos = vec3(x, y, z);
}

vec3 Chunk::getStartPos() const
{
	return this->start_pos;
}




void Chunk::readFile(string const& path)
{
}

void Chunk::updateFile() const
{
}

void Chunk::setHeight(int x, int y, int16 h)
{
	this->height_map[y][x] = h;
}

void Chunk::setHeight(Index2 const& idx2, int16 h)
{
	this->height_map[idx2.y][idx2.x] = h;
}

int16 Chunk::getHeight(int x, int y) const
{
	return this->height_map[y][x];
}

int16 Chunk::getHeight(Index2 const& idx2)
{
	return this->height_map[idx2.y][idx2.x];
}

bool Chunk::getRenderFlag()
{
	return this->render_flag;
}
