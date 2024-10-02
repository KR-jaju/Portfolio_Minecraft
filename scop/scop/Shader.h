#pragma once
#include "ResourceBase.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
class Shader : public ResourceBase
{
	using Super = ResourceBase;
public:
	Shader();
	virtual ~Shader();

	shared_ptr<InputLayout> GetInputLayout() { return this->input_layout; }
	shared_ptr<PixelShader> GetPixelShader() { return this->pixel_shader; }
	shared_ptr<VertexShader> GetVertexShader() { return this->vertex_shader; }
	void SetInputLayout(shared_ptr<InputLayout> input_layout) { this->input_layout = input_layout; }
	void SetVertexShader(shared_ptr<VertexShader> vertex_shader) { this->vertex_shader = vertex_shader; }
	void SetPixelShader(shared_ptr<PixelShader> pixel_shader) { this->pixel_shader = pixel_shader; }

private:
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;

};

