#pragma once
enum class ResourceType : uint8
{
	None = -1,
	Mesh,
	Shader,
	Texture,
	Material,
	Animation,
	End
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<uint8>(ResourceType::End)
};

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	ResourceType GetType() { return this->type; }
	void SetType(ResourceType type) { this->type = type; }

	wstring& GetName() { return this->name; }
	void SetName(const wstring& name) { this->name = name; }

	wstring& GetPath() { return this->path; }
	void SetPath(wstring& path) { this->path = path; }

	uint32 GetID() { return this->id; }
	void SetID(uint32 id) { this->id = id; }
	
	virtual void Load(const wstring& path){ }
	virtual void Save(const wstring& path){ }


private:
	ResourceType type = ResourceType::None;
	wstring name;
	wstring path;
	uint32 id = 0;
};
