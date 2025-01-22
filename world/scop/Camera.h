#pragma once

class Camera
{
public:
	Camera();
	void	setPosition(vec3 position);
	void	setRotation(vec3 position);
	void	setFov(float fov);
	vec3	getPosition() const;
	vec3	getRotation() const;
	float	getFov() const;

	Mat		getViewMatrix() const;
	Mat		getProjectionMatrix() const;
	Mat		getViewInverseMatrix() const;
private:
	vec3 position;
	vec3 rotation;
	float fov;
	float time;
};