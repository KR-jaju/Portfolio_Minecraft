#include "pch.h"
#include "Sun.h"
#include "DeferredGraphics.h"
#include "Buffer.h"

Sun::Sun(DeferredGraphics* dgraphic, UINT radius)
{
	ComPtr<ID3D11Device> device = dgraphic->getDevice();
	vector<vec3>positions;
	vector<uint32> indices;
	vector<VertexColor> vertices;

	positions.push_back(vec3(0.0f, -1.0f, 1.0f) * radius);
	positions.push_back(vec3(0.0f, 1.0f, 1.0f) * radius);
	positions.push_back(vec3(0.0f, 1.0f, -1.0f) * radius);
	positions.push_back(vec3(0.0f, -1.0f, -1.0f) * radius);
	VertexColor vertex;
	for (int i = 0; i < 4; i++) {
		vertex.pos = positions[i];
		vertex.col = color(1.0f, 1.0f, 1.0f, 1.0f);
		vertices.push_back(vertex);
	}
	indices = { 0, 1, 2, 0, 2, 3 };
	this->vbuffer = make_shared<Buffer<VertexColor>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
}

shared_ptr<Buffer<VertexColor>> Sun::getVertexBuffer()
{
	return this->vbuffer;
}

shared_ptr<Buffer<uint32>> Sun::getIndexBuffer()
{
	return this->ibuffer;
}
