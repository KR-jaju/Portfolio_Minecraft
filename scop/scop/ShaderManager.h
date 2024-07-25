#pragma once

class VertexShader;
class PixelShader;

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

private:
	map<string, shared_ptr<VertexShader>> vertex_shader_book;
	map<string, shared_ptr<PixelShader>> pixel_shader_book;
};

