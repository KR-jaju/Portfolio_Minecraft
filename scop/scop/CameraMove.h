#pragma once
#include "MonoBehaviour.h"

class InputSystem;
class TimeSystem;
class GameObject;

class CameraMove : public MonoBehaviour
{
	using Super = Component;
public:
	CameraMove(shared_ptr<InputSystem> input_system, shared_ptr<TimeSystem> time_system, shared_ptr<GameObject> camera);
	~CameraMove();
	
	virtual void Start() override;
	virtual void Update() override;

private:
	shared_ptr<InputSystem> input_system;
	shared_ptr<GameObject> camera;
	shared_ptr<TimeSystem> time_system;
	float camera_yaw = 0;
	float camera_pitch = 0;
	float speed = 10.f;
	const float rotation_sensitive = 0.0005f;
};

