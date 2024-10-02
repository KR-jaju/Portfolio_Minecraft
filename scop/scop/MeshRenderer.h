#pragma once
#include "Component.h"
class Mesh;
class Material;
#include "TextureResource.h"
#include "Material.h"
#include "Shader.h"


class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device);
	virtual ~MeshRenderer();

	void SetMaterial(shared_ptr<Material> material) { this->material = material; }
	void SetShader(shared_ptr<Shader> shader) { this->material->SetShader(shader); }
	void SetMesh(shared_ptr<Mesh> mesh) { this->mesh = mesh; }
	void SetTexture(shared_ptr<TextureResource> texture) { this->material->SetTexture(texture); }

	auto GetMaterial() { return this->material; }
	auto GetVertexShader() { return GetMaterial()->GetShader()->GetVertexShader(); }
	auto GetInputLayout() { return GetMaterial()->GetShader()->GetInputLayout(); }
	auto GetPixelShader() { return GetMaterial()->GetShader()->GetPixelShader(); }

	shared_ptr<Mesh> GetMesh() { return this->mesh; }
	shared_ptr<TextureResource> GetTexture() { return GetMaterial()->GetTexture(); }

private:
	friend class RenderManager;
	ComPtr<ID3D11Device> device;
	shared_ptr<Mesh> mesh;
	shared_ptr<Material> material;

};

