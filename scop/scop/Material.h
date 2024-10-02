#pragma once
#include "ResourceBase.h"

class Shader;
class TextureResource;

class Material : public ResourceBase
{
	using Super = ResourceBase;

public:
	Material();
	virtual ~Material();
	shared_ptr<Shader> GetShader() { return this->shader; }
	shared_ptr<TextureResource> GetTexture() { return this->texture; }

	void SetShader(shared_ptr<Shader> shader) { this->shader = shader; }
	void SetTexture(shared_ptr<TextureResource> texture) { this->texture = texture; }
private:
	shared_ptr<Shader> shader;
	shared_ptr<TextureResource> texture;
};

