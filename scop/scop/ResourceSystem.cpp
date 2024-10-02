#include "pch.h"
#include "ResourceSystem.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "TextureResource.h"
#include "model.h"

ResourceSystem::ResourceSystem(ComPtr<ID3D11Device> device)
	:device(device)
{
}

void ResourceSystem::Init()
{
	CreateDefaultTexture();
	CreateMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
}

void ResourceSystem::CreateDefaultTexture()
{
	{
		auto texture = make_shared<TextureResource>(this->device);
		texture->SetName(L"steve_texture");
		texture->Create(L"steve.png");
		this->texture_size = texture->GetSize();
		Add(texture->GetName(), texture);
	}
}

void ResourceSystem::CreateMesh()
{
	
	for (const auto& bone : model::humanoid_geometry.bone)
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>(this->device);
		mesh->SetName(bone.name);
		if (bone.BoxCheck)
		{
			for (const auto& shape : bone.shape)
			{
				mesh->CreateCube(shape.origin, shape.size, shape.uv, this->texture_size);
			}
			Add(mesh->GetName(), mesh);
		}
	}

}

void ResourceSystem::CreateDefaultShader()
{
	auto vertex_shader = make_shared<VertexShader>(this->device, L"VertexShaderUV.hlsl", "main", "vs_5_0");
	vector<D3D11_INPUT_ELEMENT_DESC> desc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//vector<D3D11_INPUT_ELEMENT_DESC> desc =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	auto input_layout = make_shared<InputLayout>(this->device, desc.data(), desc.size(), vertex_shader->getBlob());
	auto pixel_shader = make_shared<PixelShader>(this->device, L"PixelShaderUV.hlsl", "main", "ps_5_0");

	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->SetName(L"steve_shader");
	shader->SetVertexShader(vertex_shader);
	shader->SetInputLayout(input_layout);
	shader->SetPixelShader(pixel_shader);
	Add(shader->GetName(), shader);
}

void ResourceSystem::CreateDefaultMaterial()
{
	shared_ptr<Material> material = make_shared<Material>();
	material->SetName(L"steve_material");
	material->SetShader(Get<Shader>(L"steve_shader"));
	material->SetTexture(Get<TextureResource>(L"steve_texture"));
	Add(material->GetName(), material);
}
