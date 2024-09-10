#include "pch.h"
#include "TestCam.h"
#include <algorithm>

TestCam::TestCam(int width, int height)
{
	this->w_width = width;
	this->w_height = height;
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

void TestCam::onMouseMove(WPARAM btnState, int mouse_x, int mouse_y)
{
	float x = mouse_x * 2.0f / this->w_width - 1.0f;
	float y = -mouse_y * 2.0f / this->w_height + 1.0f;
	if (x < -1.f)
		x = -1.f;
	if (x > 1.f)
		x = 1.f;
	if (y < -1.f)
		y = -1.f;
	if (y > 1.f)
		y = 1.f;
	vec4 pos = vec4(x, y, 0, 1) * 0.01;
	Mat r_mat = this->mvp.proj.Invert() * this->mvp.view.Invert();
	pos = vec4::Transform(pos, r_mat);
	this->dir = vec3(pos.x, pos.y, pos.z) - this->pos;
	this->dir = XMVector3Normalize(this->dir);
	float t = this->dir.Cross(vec3(0, 1, 0)).Length();
	if (t < 0.0000001 && t > -0.0000001) {
		if (this->dir.y > 0)
			this->dir = XMVector3Normalize(vec3(0, 1, 0.001));
		else
			this->dir = XMVector3Normalize(vec3(0, -1, 0.001));
	}
}

void TestCam::setCursorInClient(HWND hwnd, int c_x, int c_y)
{
	POINT pt;
	pt.x = c_x;
	pt.y = c_y;
	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void TestCam::update()
{
	vec3 right_dir = vec3(0, 1, 0).Cross(this->dir);
	vec3 move_dir = vec3(0, 0, 0);
	if (GetAsyncKeyState('A') & 0x8000)
		move_dir -= right_dir;
	if (GetAsyncKeyState('D') & 0x8000)
		move_dir += right_dir;
	if (GetAsyncKeyState('W') & 0x8000)
		move_dir += this->dir;
	if (GetAsyncKeyState('S') & 0x8000)
		move_dir -= this->dir;
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
