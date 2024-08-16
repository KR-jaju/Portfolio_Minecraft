#include "pch.h"
#include "TestCam.h"

TestCam::TestCam()
{
}

TestCam::~TestCam()
{
}

void TestCam::movePos(float x, float y, float z)
{
	this->pos = vec3(x, y, z);
}

void TestCam::setDir(vec3 dir)
{
	this->dir = XMVector3Normalize(dir);
}

void TestCam::update()
{
	this->mvp.view = XMMatrixLookToLH(this->pos, this->dir, vec3(0, 1, 0));
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(70),
		800.f / 650.f,
		0.01f,
		1000.f
	);
}

MVP TestCam::getViewProj()
{
	return this->mvp;
}

vec3 TestCam::getPos()
{
	return this->pos;
}

vec3 TestCam::getDir()
{
	return this->dir;
}
