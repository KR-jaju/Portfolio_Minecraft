#include "pch.h"
#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(
	ComPtr<ID3D11Device> device,
	ComPtr<ID3D11DeviceContext> context	
)
	: device(device), context(context)
{
}

VertexArrayObject::~VertexArrayObject()
{
}

IndexBuffer* VertexArrayObject::getIndexBuffer() const
{
	return this->index_buffer.get();
}

VertexBuffer* VertexArrayObject::getVertexBuffer() const
{
	return this->vertex_buffer.get();
}

InputLayout* VertexArrayObject::getInputLayout() const
{
	return this->input_layout.get();
}



void VertexArrayObject::createInputLayout(
	const vector<D3D11_INPUT_ELEMENT_DESC>& descs, 
	ComPtr<ID3DBlob> vertexShaderBlob
)
{
	this->input_layout = make_unique<InputLayout>(this->device);
	this->input_layout->create(descs, vertexShaderBlob);
}

void VertexArrayObject::createIndexBuffer(
	const vector<uint32>& indices
)
{
	this->index_buffer = make_unique<IndexBuffer>(this->device);
	this->index_buffer->create(indices);
}

void VertexArrayObject::setTopology(
	D3D11_PRIMITIVE_TOPOLOGY topology
)
{
	this->topology = topology;
}

void VertexArrayObject::setInputAsembler()
{
	this->context->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();

	this->context->IASetPrimitiveTopology(this->topology);
	this->context->IASetVertexBuffers(
		0,
		1,
		this->vertex_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	this->context->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
}

