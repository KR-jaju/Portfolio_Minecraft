#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();

	virtual void Awake() override;
	virtual void Update() override;
	
	void UpdateTransform();

	//Local
	vec3 GetLocalScale() { return this->local_scale; }
	void SetLocalScale(vec3 local_scale) { this->local_scale = local_scale; UpdateTransform(); }
	vec3 GetLocalRotation() { return this->local_rotation; }
	void SetLocalRotation(vec3 local_rotation) { this->local_rotation = local_rotation; UpdateTransform();}
	vec3 GetLocalPosition() { return this->local_position; }
	void SetLocalPosition(vec3 local_position) { this->local_position = local_position; UpdateTransform();}

	//World
	vec3 GetScale() { return this->scale; }
	void SetScale(vec3 world_scale);
	vec3 GetRotation() { return this->rotation; }
	void SetRotation(vec3 world_rotation);
	vec3 GetPosition() { return this->position; }
	void SetPosition(vec3 world_position);

	vec3 GetRight() { return this->mat_world.Right(); }
	vec3 GetUp() { return this->mat_world.Up(); }
	vec3 GetLook() { return this->mat_world.Backward(); }

	
	Mat GetWorldMatrix() { return this->mat_world; }

	bool HasParent() { return this->parent != nullptr; }

	shared_ptr<Transform> GetParent() { return this->parent; }
	void SetParent(shared_ptr<Transform> parent) { this->parent = parent; }

	const vector<shared_ptr<Transform>>& GetChildren() { return this->children; }
	void AddChildren(shared_ptr<Transform> child) { this->children.push_back(child); }


private:
	vec3 local_scale = { 1.f, 1.f, 1.f };
	vec3 local_rotation = { 0.f, 0.f, 0.f };
	vec3 local_position = { 0.f, 0.f, 0.f };
	vec3 look_vec = { 0.f, 0.f, 1.f };

	Mat mat_local = Mat::Identity;
	Mat mat_world = Mat::Identity;

	vec3 scale;
	vec3 rotation;
	vec3 position;
private:
	shared_ptr<Transform> parent;
	vector<shared_ptr<Transform>> children;
};

