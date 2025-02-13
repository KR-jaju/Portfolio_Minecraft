#pragma once

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
	vec3	getPosition() const;
	vec3	getRotation() const;
	float	getFov() const;

	void	updateMatrices();

	Mat const&	getViewMatrix() const;
	Mat const&	getProjectionMatrix() const;
	Mat const& getViewInverseMatrix() const;
	Mat const&	getViewInverseTransposeMatrix() const;
private:
	Mat view;
	Mat projection;
	Mat view_inverse;
	Mat view_inverse_transpose;

	vec3 position;
	vec3 rotation;
	float fov;
	float time;
};