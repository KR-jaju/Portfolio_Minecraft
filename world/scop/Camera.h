#pragma once

#include "Frustum.h"

/*
TODO : 위치가 애매함, 개선 예정
*/

class Camera
{
public:
	Camera();
	void	setPosition(vec3 position);
	void	setRotation(vec3 position);
	void	setFov(float fov);
	void	setSize(int width, int height);
	void	setDepthRange(float near_plane, float far_plane);
	vec3	getPosition() const;
	vec3	getRotation() const;
	float	getFov() const;

	void	update();

	Mat const&	getViewMatrix() const;
	Mat const&	getProjectionMatrix() const;
	Mat const&	getViewInverseMatrix() const;
	Mat const&	getViewInverseTransposeMatrix() const;
	Mat const&	getProjectionInverseMatrix() const;
	Frustum const& getFrustum() const;
private:
	Mat view;
	Mat projection;
	Mat view_inverse;
	Mat view_inverse_transpose;
	Mat projection_inverse;
	Frustum frustum;
	vec3 left;
	vec3 up;
	vec3 forward;

	vec4 test;

	vec3 position;
	vec3 rotation;
	float fov;
	float near_plane;
	float far_plane;
	float width;
	float height;
	float aspect_ratio;
	
	void	updateFrustum();
	void	updateMatrices();
};