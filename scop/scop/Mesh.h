#pragma once
#include "ResourceBase.h"
#include "Buffer.h"
#include "Geometry.h"
#include "VertexData.h"

class Mesh : public ResourceBase
{
	using Super = ResourceBase;
public:
	Mesh(ComPtr<ID3D11Device> device);
	virtual ~Mesh();
	
	void CreateCube(vec3 origin, vec3 size, vec2 uv, vec2 texture_size);
	void  CreateColorCube(vec3 origin, vec3 size);


	shared_ptr<Buffer<VertexUV>> GetVertexBuffer() { return this->vertex_buffer; }
	shared_ptr<Buffer<uint32>> GetIndexBuffer() { return this->index_buffer; }
	shared_ptr<Buffer<Vertex>> GetVertexColorBuffer() { return this->vertex_color_buffer; }
private:
	ComPtr<ID3D11Device> device;
	shared_ptr<Geometry<VertexUV>> geometry;
	shared_ptr<Geometry<Vertex>> geometry_color;
	shared_ptr<Buffer<VertexUV>> vertex_buffer;
	shared_ptr<Buffer<Vertex>> vertex_color_buffer;
	shared_ptr<Buffer<uint32>> index_buffer;
};
