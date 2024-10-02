#include "pch.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

Camera::Camera(float camera_width, float camera_height)
	:Super(ComponentType::Camera), camera_width(camera_width), camera_height(camera_height)
{

}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	vec3 eye_position = GetTransform()->GetPosition();
	//vec3 focus_position = GetTransform()->GetLook() - eye_position_normal;
	vec3 focus_position = eye_position + GetTransform()->GetLook();
	vec3 up_direction = GetTransform()->GetUp();
	this->mvp.view = ::XMMatrixLookAtLH(eye_position, focus_position, up_direction);
	this->mvp.proj = ::XMMatrixPerspectiveFovLH(this->camera_fov, this->camera_width / this->camera_height, this->camera_near, this->camera_far);
}