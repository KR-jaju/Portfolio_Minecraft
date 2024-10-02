#include "pch.h"
#include "Transform.h"

Transform::Transform()
	:Component(ComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::Awake()
{
}

void Transform::Update()
{
}
// quaternion을 vec3로 바꿔주는 함수
vec3 ToEulerAngles(Quaternion q)
{
	vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateTransform()
{
	Mat mat_scale = Mat::CreateScale(this->local_scale);
	Mat mat_rotation = Mat::CreateRotationX(this->local_rotation.x);
	mat_rotation *= Mat::CreateRotationY(this->local_rotation.y);
	mat_rotation *= Mat::CreateRotationZ(this->local_rotation.z);
	Mat mat_translation = Mat::CreateTranslation(this->local_position);

	this->mat_local = mat_scale * mat_rotation * mat_translation;

	if (HasParent())
	{
		this->mat_world = mat_local * this->parent->GetWorldMatrix();
	}
	else
	{
		this->mat_world = this->mat_local;
	}
	Quaternion quat;
	this->mat_world.Decompose(this->scale, quat, this->position);
	this->rotation = ToEulerAngles(quat);

	//Children
	for (shared_ptr<Transform> child : this->children)
		child->UpdateTransform();

}

void Transform::SetScale(vec3 world_scale)
{
	if (HasParent())
	{
		vec3 parent_scale = this->parent->GetScale();
		vec3 set_scale = world_scale;
		set_scale.x /= parent_scale.x;
		set_scale.y /= parent_scale.y;
		set_scale.z /= parent_scale.z;
		SetLocalScale(set_scale);
	}
	else
	{
		SetLocalScale(world_scale);
	}
}

void Transform::SetRotation(vec3 world_rotation)
{
	if (HasParent())
	{
		Mat inverse_matrix = this->parent->GetWorldMatrix().Invert();

		vec3 rotation;
		rotation.TransformNormal(world_rotation, inverse_matrix);

		SetLocalRotation(rotation);
	}
	else
		SetLocalRotation(world_rotation);
}

void Transform::SetPosition(vec3 world_position)
{
	if (HasParent())
	{
		Mat inverse_matrix = this->parent->GetWorldMatrix().Invert();

		vec3 position;
		position.Transform(world_position, inverse_matrix);

		SetLocalPosition(position);
	}
	else
		SetLocalPosition(world_position);
}
