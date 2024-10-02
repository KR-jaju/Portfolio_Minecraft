#pragma once

class GameObject;
class Transform;

enum class ComponentType : uint8
{
	Transform,
	MeshRenderer,
	Camera,
	script,
	End,

};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1
};

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FixedUpdate() { }

	ComponentType GetType() { return this->type; }
	shared_ptr<Transform> GetTransform();
	shared_ptr<GameObject> GetGameObject();
	void SetGameObject(shared_ptr<GameObject> game_object) { this->game_object = game_object; }


private:
	friend class GameObject;
	ComponentType type;
	weak_ptr<GameObject> game_object;
};

