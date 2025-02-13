#include "pch.h"
#include "Light.h"
#include "AABB.h"

DirectionalLight::DirectionalLight()
	: view_projection(),
	direction(0, -1, 0),
	color(1, 1, 1)
{

}

static float calculateSplitPlane(float near_plane, float far_plane, float t)
{
	float const ratio = 0.6f;
	float c_log = log(near_plane * pow(far_plane / near_plane, t)); // 로그 기반 분할 (0일 때 near가 아닌게 의문)
	float c_uni = near_plane + (far_plane - near_plane) * (t); // 선형 기반 분할
	float c = ratio * c_log + (1.0 - ratio) * c_uni; // 혼합 방식 적용

	return (c);
}

void	DirectionalLight::update(Camera const& camera)
{
	Mat const& projection = camera.getProjectionMatrix();
	Mat const& inv_view = camera.getViewInverseMatrix();
	int const levels = 4;
	vec3 const forward = vec3::TransformNormal(vec3(0, 0, 1), inv_view);
	float const near_plane = projection._43 / (projection._33 - 1.0f);
	float const far_plane = projection._43 / (projection._33 + 1.0f);
	float height = 1.0f / projection._21;
	float width = height * projection._21 / projection._11;
	Mat light_view = Mat::CreateLookAt(this->direction, -this->direction, forward); // TODO: 일단 와드

	for (int level = 0; level < levels; ++level)
	{
		float near_t = calculateSplitPlane(near_plane, far_plane, static_cast<float>(level) / levels);
		float far_t = calculateSplitPlane(near_plane, far_plane, static_cast<float>(level + 1) / levels);
		AABB projection_box = {vec3::Transform(vec3::Transform(vec3(-width, -height, near_t), inv_view), light_view), vec3::Zero};

		projection_box.expand(vec3::Transform(vec3::Transform(vec3(width, -height, near_t), inv_view), light_view));
		projection_box.expand(vec3::Transform(vec3::Transform(vec3(-width, height, near_t), inv_view), light_view));
		projection_box.expand(vec3::Transform(vec3::Transform(vec3(width, -height, near_t), inv_view), light_view));
		projection_box.expand(vec3::Transform(vec3::Transform(vec3(-width, -height, far_t), inv_view), light_view));
		projection_box.expand(vec3::Transform(vec3::Transform(vec3(width, -height, far_t), inv_view), light_view));
		projection_box.expand(vec3::Transform(vec3::Transform(vec3(-width, height, far_t), inv_view), light_view));
		projection_box.expand(vec3::Transform(vec3::Transform(vec3(width, -height, far_t), inv_view), light_view));

		vec3 min = projection_box.center - projection_box.extents;
		vec3 max = projection_box.center + projection_box.extents;
		this->view_projection[level] = light_view * Mat::CreateOrthographicOffCenter(min.x, max.x, min.y, max.y, min.z - 200, max.z);
	}
}

vec3 DirectionalLight::getDirection() const
{
	return (this->direction);
}

vec3 DirectionalLight::getColor() const
{
	return (this->color);
}

Mat const& DirectionalLight::getViewProjectionMatrix(int level) const
{
	return (this->view_projection[level]);
}
