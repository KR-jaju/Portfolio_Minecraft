#include "pch.h"
#include "Transform.h"

Mat	Transform::toMatrix() const
{
	Mat t = Mat::CreateTranslation(this->position);
	Mat r = Mat::CreateFromQuaternion(this->rotation);
	Mat s = Mat::CreateScale(this->scale);
	
	return (s * r * t);
}