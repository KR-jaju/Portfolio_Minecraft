#pragma once
#include "Component.h"

class Camera : public Component
{
	using Super = Component;
public:
	Camera(float camera_width, float camera_height);
	virtual ~Camera();

	virtual void Update() override;

	void UpdateMatrix();

	void SetNear(float value) { this->camera_near = value; }
	void SetFar(float value) { this->camera_far = value; }
	void SetFov(float value) { this->camera_fov = value; }
	void SetWidth(float value) { this->camera_width = value; }
	void SetHeight(float value) { this->camera_height = value; }
	float GetNear() { return this->camera_near; }
	float GetFar() { return this->camera_far; }
	float GetFov() { return this->camera_fov; }
	float GetWidth(float value) { return this->camera_width; }
	float GetHeight(float value) { return this->camera_height; }

	Mat& GetViewMatrix() { return this->mvp.view; }
	Mat& GetModelMatrix() { return this->mvp.model; }
	void SetViewMatrix(Mat mat_view) { this->mvp.view = mat_view; }
	Mat& GetProjectionMatrix() { return this->mvp.proj; }
	void SetProjectionMatrix(Mat mat_projection) { this->mvp.proj = mat_projection; }
	MVP& GetMVP() { return this->mvp; }
	void SetMVP(MVP mvp) { this->mvp = mvp; }
private:
	MVP mvp; // mvp model, mvp view, mvp.proj
	float camera_near = 0.1f;
	float camera_far = 1000.f;
	float camera_fov = XMConvertToRadians(70);
	float camera_width = 800.f;
	float camera_height = 650.f;	
};

