#include "pch.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"

ShaderManager::ShaderManager(ComPtr<ID3D11Device> device)
	: device(device)
{
}

ShaderManager::~ShaderManager()
{
}

shared_ptr<VertexShader> ShaderManager::getVertexShader(string name)
{
	if (this->checkShaderBook(name, 0) == false)
		return nullptr;
	return this->vertex_shader_book[name];
}

shared_ptr<PixelShader> ShaderManager::getPixelShader(string name)
{
	if (this->checkShaderBook(name, 1) == false)
		return nullptr;
	return this->pixel_shader_book[name];
}

bool ShaderManager::addVertexShader(
	string shader_name, 
	const wstring& path, 
	const string& entrypoint_name, 
	const string& version
)
{
	if (this->checkShaderBook(shader_name, 0))
		return false;
	shared_ptr<VertexShader> vertex_shader;
	vertex_shader = make_shared<VertexShader>(this->device);
	vertex_shader->create(path, entrypoint_name, version);
	this->vertex_shader_book[shader_name] = vertex_shader;
	return true;
}

bool ShaderManager::addPixelShader(
	string shader_name, 
	const wstring& path, 
	const string& entrypoint_name, 
	const string& version
)
{
	if (this->checkShaderBook(shader_name, 1))
		return false;
	shared_ptr<PixelShader> pixel_shader;
	pixel_shader = make_shared<PixelShader>(this->device);
	pixel_shader->create(path, entrypoint_name, version);
	this->pixel_shader_book[shader_name] = pixel_shader;
	return true;
}

bool ShaderManager::removePixelShader(string name)
{
	if (this->checkShaderBook(name, 1)) {
		this->pixel_shader_book.erase(name);
		return true;
	}
	return false;
}

bool ShaderManager::removeVertexShader(string name)
{
	if (this->checkShaderBook(name, 0)) {
		this->vertex_shader_book.erase(name);
		return true;
	}
	return false;
}


bool ShaderManager::checkShaderBook(string name, int flag) const
{
	if (flag == 0) {
		map<string, shared_ptr<VertexShader>>::const_iterator it;
		it = this->vertex_shader_book.find(name);
		if (it == this->vertex_shader_book.end())
			return false;
		return true;
	}
	else {
		map<string, shared_ptr<PixelShader>>::const_iterator it;
		it = this->pixel_shader_book.find(name);
		if (it == this->pixel_shader_book.end())
			return false;
		return true;
	}
}
