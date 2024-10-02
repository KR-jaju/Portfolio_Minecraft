#pragma once
#include "Graphics.h"
#include "ResourceSystem.h"
#include "TimeSystem.h"
#include "InputSystem.h"
class Scene;

class SceneSystem
{
public:
	SceneSystem(shared_ptr<Graphics> graphics, 
		shared_ptr<ResourceSystem> resource_system, 
		shared_ptr<InputSystem> input_system, 
		shared_ptr<TimeSystem> time_system);

	void Init();
	void Update();
	void LoadScene(wstring scene_name);
public:
	shared_ptr<Scene> GetActiveScene() { return active_scene; }
private:
	shared_ptr<Scene> LoadMinecraftScene();
private:
	shared_ptr<Graphics> graphics;
	shared_ptr<ResourceSystem> resource_system;
	shared_ptr<InputSystem> input_system;
	shared_ptr<TimeSystem> time_system;
private:
	shared_ptr<Scene> active_scene;
};

