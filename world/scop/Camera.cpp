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

void	Camera::updateMatrices()
{
	vec3 position = this->position;
	vec3 rotation = this->rotation;
	Mat t = Mat::CreateTranslation(position.x, position.y, position.z);
	Mat r = Mat::CreateFromYawPitchRoll(rotation.y + 3.141592f, rotation.x, rotation.z); // 180도 더하는건 z+가 앞으로 가게 하기 위함임!
	Mat inv_r = r.Transpose(); // 180도 더하는건 z+가 앞으로 가게 하기 위함임!
	Mat inv_t = Mat::CreateTranslation(-position.x, -position.y, -position.z);
	float fov = this->fov;
	float aspect_ratio = 1.0f;

	this->view = inv_t * inv_r;
	this->projection = Mat::CreatePerspectiveFieldOfView(fov, aspect_ratio, 0.3f, 500.0f);
	this->view_inverse = r * t;
}

Mat const&	Camera::getViewMatrix() const
{
	return (this->view);
}

Mat const&	Camera::getProjectionMatrix() const
{
	return (this->projection);
}

Mat const& Camera::getViewInverseMatrix() const
{
	return (this->view_inverse);
}

Mat const&	Camera::getViewInverseTransposeMatrix() const
{
	return (this->view_inverse.Transpose());
}
/*
(inv_t * inv_r)^-1^t
(r * t) ^ t
t^t * r^t





*/