#include "pch.h"
#include "Camera.h"

Camera::Camera()
	: position(0, 0, 0),
	rotation(0, 0, 0),
	fov(1.6f),
	near_plane(0.3f), far_plane(500.0f),
	width(800), height(800), aspect_ratio(1)
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

void	Camera::setSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void	Camera::setDepthRange(float near_plane, float far_plane)
{
	this->near_plane = near_plane;
	this->far_plane = far_plane;
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

static vec4	calculatePlane(vec3 const& normal, vec3 const& point)
{
	float inv_mag = 1.0f / normal.Length();
	vec3 normalized_normal(normal.x * inv_mag, normal.y * inv_mag, normal.z * inv_mag);

	return vec4(
		normalized_normal.x, normalized_normal.y, normalized_normal.z,
		-(normalized_normal.x * point.x + normalized_normal.y * point.y + normalized_normal.z * point.z)
	);
}

void	Camera::update()
{
	this->aspect_ratio = this->width / this->height;
	this->updateMatrices();
	this->updateFrustum();
}

void	Camera::updateFrustum()
{
	const float v_slope = tanf(this->fov * 0.5f);
	const float h_slope = this->aspect_ratio * v_slope;
	const vec3 scaled_forward = this->near_plane * this->forward;
	const vec3 scaled_left = this->near_plane * h_slope * this->left;
	const vec3 scaled_up = this->near_plane * v_slope * this->up;
	
	this->frustum.near_plane = calculatePlane(this->forward, this->position + this->forward * this->near_plane);
	this->frustum.far_plane = calculatePlane(-this->forward, this->position + this->forward * this->far_plane);
	
	this->frustum.left_plane = calculatePlane((scaled_forward + scaled_left).Cross(this->up), this->position);
	this->frustum.right_plane = calculatePlane((this->up).Cross(scaled_forward - scaled_left), this->position);

	this->frustum.top_plane = calculatePlane((this->left).Cross(scaled_forward + scaled_up), this->position);
	this->frustum.bottom_plane = calculatePlane((scaled_forward - scaled_up).Cross(this->left), this->position);
}


Frustum const& Camera::getFrustum() const
{
	return (this->frustum);
}

void	Camera::updateMatrices()
{
	vec3 position = this->position;
	vec3 rotation = this->rotation;
	Mat t = Mat::createTranslation(position.x, position.y, position.z);
	Mat r = Mat::createRotation(rotation.y, rotation.x, rotation.z);
	Mat inv_r = r.transpose();
	Mat inv_t = Mat::createTranslation(-position.x, -position.y, -position.z);

	this->view = inv_t * inv_r;
	this->projection = Mat::createPerspective(this->fov, this->aspect_ratio, 0.3f, 500.0f);
	this->view_inverse = r * t;
	this->projection_inverse.m[0] = 1.0f / this->projection.m[0]; // 1/h
	this->projection_inverse.m[5] = 1.0f / this->projection.m[5]; // 1/v
	this->projection_inverse.m[11] = 1.0f;
	this->projection_inverse.m[14] = 1.0f / this->projection.m[11];
	this->projection_inverse.m[15] = -this->projection.m[10] / this->projection.m[11];

	this->left = vec3(1, 0, 0) * this->view_inverse;
	this->up = vec3(0, 1, 0) * this->view_inverse;
	this->forward = vec3(0, 0, 1) * this->view_inverse;
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
	return (this->view_inverse.transpose());
}

Mat const& Camera::getProjectionInverseMatrix() const
{
	return (this->projection_inverse.transpose());
}

