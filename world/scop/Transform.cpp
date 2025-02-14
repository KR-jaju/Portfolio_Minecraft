#include "pch.h"
#include "Transform.h"

Mat	Transform::toMatrix() const
{
	Mat t = Mat::createTranslation(this->position);
	Mat r = Mat::createFromQuaternion(this->rotation);
	Mat s = Mat::createScale(this->scale);
	
	return (s * r * t);
}