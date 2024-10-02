#pragma once
#include "ResourceBase.h"
#include "Mesh.h"

class Material;
class Shader;
class Animation;
class TextureResource;


class ResourceSystem
{
public:
	ResourceSystem(ComPtr<ID3D11Device> device);

	void Init();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);
	
	template<typename T>
	ResourceType GetResourceType();
private:
	void CreateDefaultTexture();
	void CreateMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
private:
	ComPtr<ID3D11Device> device;
	vec2 texture_size = { 0.f , 0.f };
	using KeyObjMap = map<wstring, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> resources;
};

template<typename T>
shared_ptr<T> ResourceSystem::Load(const wstring& key, const wstring& path)
{
	auto object_type = GetResourceType<T>();
	KeyObjMap& key_obj_map = this->resources[static_cast<uint8>(object_type)];

	auto find_it = key_obj_map.find(key);
	if (find_it != key_obj_map.end())
		return static_pointer_cast<T>(find_it->second);
	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	key_obj_map[key] = object;

	return object;
}


template<typename T>
bool ResourceSystem::Add(const wstring& key, shared_ptr<T> object)
{
	ResourceType resource_type = GetResourceType<T>();
	KeyObjMap& key_obj_map = this->resources[static_cast<uint8>(resource_type)];

	auto find_it = key_obj_map.find(key);
	if (find_it != key_obj_map.end())
		return false;
	key_obj_map[key] = object;
	return true;
}

template<typename T>
shared_ptr<T> ResourceSystem::Get(const wstring& key)
{
	ResourceType resource_type = GetResourceType<T>();
	KeyObjMap& key_obj_map = this->resources[static_cast<uint8>(resource_type)];

	auto find_it = key_obj_map.find(key);
	if (find_it != key_obj_map.end())
		return static_pointer_cast<T>(find_it->second);

	return nullptr;
}

template<typename T>
ResourceType ResourceSystem::GetResourceType()
{
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, TextureResource>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;
	if (std::is_same_v<T, Animation>)
		return ResourceType::Animation;

	assert(false);
	return ResourceType::None;
}
