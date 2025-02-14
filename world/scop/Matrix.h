#pragma once

/*
SimpleMath 쓰다가 헷갈려 죽을거같아서 직접 만드는 Column major matrix
*/
using vec2 = DirectX::SimpleMath::Vector2;
using vec3 = DirectX::SimpleMath::Vector3;
using vec4 = DirectX::SimpleMath::Vector4;
using Quaternion = DirectX::XMVECTOR;

struct Matrix
{
	float m[16];

	static constexpr Matrix identity();

	static Matrix createPerspective(float fov, float aspect_ratio, float near_plane, float far_plane);
	static Matrix createTranslation(float x, float y, float z);
	static Matrix createTranslation(vec3 translation);
	static Matrix createRotationX(float angle);
	static Matrix createRotationY(float angle);
	static Matrix createRotationZ(float angle);
	static Matrix createRotation(float yaw, float pitch, float roll);
	static Matrix createFromQuaternion(Quaternion quaternion);
	static Matrix createScale(float x, float y, float z);
	static Matrix createScale(vec3 scale);
	
	Matrix operator*(Matrix const& other) const;
	Matrix transpose() const;
};

vec3 operator*(vec3 const& v, Matrix const& matrix);
vec4 operator*(vec4 const& v, Matrix const& matrix);
