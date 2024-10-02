#pragma once

class GameObject;

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

public:
	void AddGameObject(shared_ptr<GameObject> game_object);
	void RemoveGameObject(shared_ptr<GameObject> game_object);

	const vector<shared_ptr<GameObject>>& GetGameObjects() { return this->game_objects; }

private:
	vector<shared_ptr<GameObject>> game_objects;
};

