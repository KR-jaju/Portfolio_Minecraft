#include "pch.h"
#include "Camera.h"

Camera::Camera()
	: fov(1.6f)
{

}

void	Camera::setPosition(vec3 position)
{
	this->position = position;
}
void	Camera::setRotation(vec3 rotation)
{
	this->rotation = rotation;
}
void	Camera::setFov(float fov)
{
	this->fov = fov;
}

vec3	Camera::getPosition() const
{
	return (this->position);
}

vec3	Camera::getRotation() const
{
	return (this->rotation);
}

float	Camera::getFov() const
{
	return (this->fov);
}

Mat		Camera::getViewMatrix() const
{
	vec3 position = this->position;
	vec3 rotation = this->rotation;
	Mat inv_r = Mat::CreateFromYawPitchRoll(rotation.y + 3.141592f, rotation.x, rotation.z).Transpose(); // 180도 더하는건 z+가 앞으로 가게 하기 위함임!
	Mat inv_t = Mat::CreateTranslation(-position.x, -position.y, -position.z);

	return (inv_t * inv_r);
}

Mat		Camera::getProjectionMatrix() const
{
	float fov = this->fov;
	float aspect_ratio = 1.0f;

	return Mat::CreatePerspectiveFieldOfView(fov, aspect_ratio, 0.3f, 500.0f);
}