#pragma once
#include "InputLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArrayObject
{
public:
	VertexArrayObject(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context
	);
	~VertexArrayObject();
	IndexBuffer* getIndexBuffer() const;
	VertexBuffer* getVertexBuffer() const;
	InputLayout* getInputLayout() const;
	
	template<typename T>
	void createVertexBuffer(const vector<T>& vertices) {
		this->vertex_buffer = make_unique<VertexBuffer>(this->device);
		this->vertex_buffer->create(vertices);
	}
	void createInputLayout(
		const vector<D3D11_INPUT_ELEMENT_DESC>& descs,
		ComPtr<ID3DBlob> vertexShaderBlob
	);
	void createIndexBuffer(const vector<uint32>& indices);
	void setTopology(
		D3D11_PRIMITIVE_TOPOLOGY topology = 
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	void setInputAsembler();
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	unique_ptr<IndexBuffer> index_buffer;
	unique_ptr<VertexBuffer> vertex_buffer;
	unique_ptr<InputLayout> input_layout;
	D3D11_PRIMITIVE_TOPOLOGY topology =
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

