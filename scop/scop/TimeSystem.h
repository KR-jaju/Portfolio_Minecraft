#pragma once

class TimeSystem
{

public:
	TimeSystem();
	~TimeSystem();
	void Update();

	uint32 GetFps() { return this->fps; }
	float GetDeltaTime() { return this->deltaTime; }

private:
	uint64	frequency = 0;
	uint64	prevCount = 0;
	float	deltaTime = 0.f;

private:
	uint32	frameCount = 0;
	float	frameTime = 0.f;
	uint32	fps = 0;
};

