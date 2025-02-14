#include "pch.h"
#include "Matrix.h"
#include <cmath>

constexpr Matrix Matrix::identity()
{
	Matrix result = {};

	result.m[0] = 1.0f;
	result.m[5] = 1.0f;
	result.m[10] = 1.0f;
	result.m[15] = 1.0f;
	return (result);
}

Matrix Matrix::createPerspective(float fov, float aspect_ratio, float near_plane, float far_plane)
{
	float const v_slope = 1.0f / tanf(fov * 0.5f); // vertical fov
	float const h_slope = v_slope / aspect_ratio;
	Matrix result = {};

	result.m[0] = -h_slope;
	result.m[5] = v_slope;
	result.m[10] = far_plane / (far_plane - near_plane);
	result.m[11] = -near_plane * far_plane / (far_plane - near_plane);
	result.m[14] = 1.0f; // clip_space_w = view_space_z;
	return (result);
}

Matrix Matrix::createTranslation(float x, float y, float z)
{
	Matrix result = Matrix::identity();

	result.m[3] = x;
	result.m[7] = y;
	result.m[11] = z;
	return (result);
}

Matrix Matrix::createTranslation(vec3 translation)
{
	return (Matrix::createTranslation(translation.x, translation.y, translation.z));
}

Matrix Matrix::createRotationX(float angle)
{
	float const cosine = cosf(angle);
	float const sine = sinf(angle);
	Matrix result = {};

	result.m[0] = 1.0f;
	result.m[5] = cosine;
	result.m[6] = sine;
	result.m[9] = -sine;
	result.m[10] = cosine;
	result.m[15] = 1.0f;
	return (result);
}

Matrix Matrix::createRotationY(float angle)
{
	float const cosine = cosf(angle);
	float const sine = sinf(angle);
	Matrix result = {};

	result.m[0] = cosine;
	result.m[2] = -sine;
	result.m[5] = 1.0f;
	result.m[8] = sine;
	result.m[10] = cosine;
	result.m[15] = 1.0f;
	return (result);
}

Matrix Matrix::createRotationZ(float angle)
{
	float const cosine = cosf(angle);
	float const sine = sinf(angle);
	Matrix result = {};

	result.m[0] = cosine;
	result.m[1] = sine;
	result.m[4] = -sine;
	result.m[5] = cosine;
	result.m[10] = 1.0f;
	result.m[15] = 1.0f;
	return (result);
}

Matrix Matrix::createRotation(float yaw, float pitch, float roll)
{
	return Matrix::createRotationZ(roll) * Matrix::createRotationX(pitch) * Matrix::createRotationY(yaw);
}

Matrix Matrix::createFromQuaternion(Quaternion quaternion)
{
	return (Matrix::identity()); // TODO: 복잡해서 일단 미룸
}

Matrix Matrix::createScale(float x, float y, float z)
{
	Matrix result = {};

	result.m[0] = x;
	result.m[5] = y;
	result.m[10] = z;
	result.m[15] = 1.0f;
	return (result);
}

Matrix Matrix::createScale(vec3 scale)
{
	return (Matrix::createScale(scale.x, scale.y, scale.z));
}

Matrix Matrix::operator*(Matrix const& other) const
{
	Matrix result;

	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			result.m[col * 4 + row] =
				this->m[0 * 4 + row] * other.m[col * 4 + 0] +
				this->m[1 * 4 + row] * other.m[col * 4 + 1] +
				this->m[2 * 4 + row] * other.m[col * 4 + 2] +
				this->m[3 * 4 + row] * other.m[col * 4 + 3];
		}
	}
	return (result);
}


Matrix Matrix::transpose() const
{
	Matrix result;

	for (int column = 0; column < 4; ++column)
	{
		for (int row = 0; row < 4; ++row)
		{
			result.m[row * 4 + column] = this->m[column * 4 + row];
		}
	}
	return (result);
}

vec3 operator*(vec3 const& v, Matrix const& matrix)
{
	return vec3(
		v.x * matrix.m[0] + v.y * matrix.m[1] + v.z * matrix.m[2],
		v.x * matrix.m[4] + v.y * matrix.m[5] + v.z * matrix.m[6],
		v.x * matrix.m[8] + v.y * matrix.m[9] + v.z * matrix.m[10]
	);
}

vec4 operator*(vec4 const& v, Matrix const& matrix)
{
	return vec4(
		v.x * matrix.m[0] + v.y * matrix.m[1] + v.z * matrix.m[2] + v.w * matrix.m[3],
		v.x * matrix.m[4] + v.y * matrix.m[5] + v.z * matrix.m[6] + v.w * matrix.m[7],
		v.x * matrix.m[8] + v.y * matrix.m[9] + v.z * matrix.m[10] + v.w * matrix.m[11],
		v.x * matrix.m[12] + v.y * matrix.m[13] + v.z * matrix.m[14] + v.w * matrix.m[15]
	);
}