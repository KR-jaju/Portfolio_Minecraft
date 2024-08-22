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
	vec3 move_dir = vec3(0, 0, 0);
	if (GetAsyncKeyState('A') & 0x8000)
		move_dir += vec3(-1, 0, 0);
	if (GetAsyncKeyState('D') & 0x8000)
		move_dir += vec3(1, 0, 0);
	if (GetAsyncKeyState('W') & 0x8000)
		move_dir += vec3(0, 0, 1);
	if (GetAsyncKeyState('S') & 0x8000)
		move_dir += vec3(0, 0, -1);
	move_dir = XMVector3Normalize(move_dir) * 0.3f;
	this->pos += move_dir;
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
