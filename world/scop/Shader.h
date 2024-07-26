#pragma once
class Shader
{
public:
	Shader();
	virtual ~Shader();
	ComPtr<ID3DBlob> getBlob() const;
	virtual void create(
		const wstring& path,
		const string& name,
		const string& version
	) abstract;

protected:
	void loadShaderFromFile(
		const wstring& path,
		const string& name,
		const string& version
	);

protected:
	ComPtr<ID3DBlob> blob;
};

