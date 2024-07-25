#pragma once

class VertexShader;
class PixelShader;

class ShaderManager
{
public:
	ShaderManager(ComPtr<ID3D11Device> device);
	~ShaderManager();
	shared_ptr<VertexShader> getVertexShader(string name);
	shared_ptr<PixelShader> getPixelShader(string name);
	bool addVertexShader(
		string shader_name,
		const wstring& path,
		const string& entrypoint_name,
		const string& version
	);
	bool addPixelShader(
		string shader_name,
		const wstring& path,
		const string& entrypoint_name,
		const string& version
	);
	bool removePixelShader(string name);
	bool removeVertexShader(string name);
private:
	bool checkShaderBook(string name, int flag) const;

private:
	ComPtr<ID3D11Device> device;
	map<string, shared_ptr<VertexShader>> vertex_shader_book;
	map<string, shared_ptr<PixelShader>> pixel_shader_book;
};

