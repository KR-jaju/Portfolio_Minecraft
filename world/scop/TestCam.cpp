#include "pch.h"
#include "TestCam.h"
#include <algorithm>

TestCam::TestCam(
	float width, 
	float height, 
	float fov,
	float cam_near, 
	float cam_far
)
{
	this->w_width = width;
	this->w_height = height;
	this->fov = fov;
	this->setDir(vec3(0, 0, 1));
	this->mvp.view = Mat::Identity;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(fov),
		width / height,
		cam_near,
		cam_far
	);
	this->cam_near = cam_near;
	this->cam_far = cam_far;
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

double TestCam::ndcToDegree(double ndc)
{
	return this->fov / 2 * ndc;
}


void TestCam::onMouseMove(HWND hWnd, int mouse_x, int mouse_y)
{
	float x = mouse_x * 2.0 / this->w_width - 1.0;
	float y = -mouse_y * 2.0 / this->w_height + 1.0;
	if (x < -1.f)
		x = -1.f;
	if (x > 1.f)
		x = 1.f;
	if (y < -1.f)
		y = -1.f;
	if (y > 1.f)
		y = 1.f;
	// 각도로 
	/*{
		double y_degree = ndcToDegree(x);
		double x_degree = ndcToDegree(-y);
		this->dir = vec3::Transform(this->dir,
			Mat::CreateRotationY(XMConvertToRadians(y_degree)) *
			Mat::CreateRotationX(XMConvertToRadians(x_degree)));
	}*/
	
	// 역행렬으로 
	/*{
		vec4 pos = vec4(x, y, 0, 1) * this->cam_near;
		vec4 ppos = vec4(x, y, 1, 1) * this->cam_far;
		Mat r_mat = (this->mvp.view * this->mvp.proj).Invert();
		pos = vec4::Transform(pos, r_mat);
		ppos = vec4::Transform(ppos, r_mat);
		vec4 ddir = ppos - pos;
		this->dir = vec3(ddir.x, ddir.y, ddir.z);
		this->dir.Normalize();
	}*/

	// vec3
	{
		vec3 npos = vec3(x, y, 0);
		vec3 fpos = vec3(x, y, 1);
		Mat invViewProj = (this->mvp.view * this->mvp.proj).Invert();
		npos = vec3::Transform(npos, invViewProj);
		fpos = vec3::Transform(fpos, invViewProj);
		this->dir = fpos - npos;
		this->dir.Normalize();
	}
	float t = this->dir.Cross(vec3(0, 1, 0)).Length();
	if (t < 0.0000001 && t > -0.0000001) {
		if (this->dir.y > 0)
			this->dir = XMVector3Normalize(vec3(0, 1, 0.001));
		else
			this->dir = XMVector3Normalize(vec3(0, -1, 0.001));
	}
	this->mvp.view = XMMatrixLookToLH(this->pos, this->dir, vec3(0, 1, 0));
}

void TestCam::setCursorInClient(HWND hwnd)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	// 클라이언트 영역의 중앙 좌표 계산
	int c_x = (clientRect.right - clientRect.left) / 2;
	int c_y = (clientRect.bottom - clientRect.top) / 2;

	POINT pt;
	pt.x = c_x;
	pt.y = c_y;
	ClientToScreen(hwnd, &pt);

	SetCursorPos(pt.x, pt.y);
}

void TestCam::update()
{
	vec3 right_dir = vec3(0, 1, 0).Cross(this->dir);
	vec3 up_dir = this->dir.Cross(right_dir);
	vec3 move_dir = vec3(0, 0, 0);
	if (GetAsyncKeyState('A') & 0x8000)
		move_dir -= vec3(right_dir.x, 0, right_dir.z);
	if (GetAsyncKeyState('D') & 0x8000)
		move_dir += vec3(right_dir.x, 0, right_dir.z);
	if (GetAsyncKeyState('W') & 0x8000)
		move_dir += vec3(this->dir.x, 0, this->dir.z);
	if (GetAsyncKeyState('S') & 0x8000)
		move_dir -= vec3(this->dir.x, 0, this->dir.z);
	if (GetAsyncKeyState('Q') & 0x8000)
		move_dir += vec3(0, 1, 0);
	if (GetAsyncKeyState('E') & 0x8000)
		move_dir -= vec3(0, 1, 0);
	move_dir = XMVector3Normalize(move_dir) * 0.03f;
	this->pos += move_dir;
	this->mvp.view = XMMatrixLookToLH(this->pos, this->dir, vec3(0, 1, 0));
}

void TestCam::setNear(float cam_near)
{
	this->cam_near = cam_near;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(this->fov),
		this->w_width / this->w_height,
		this->cam_near,
		this->cam_far
	);
}

void TestCam::setFar(float cam_far)
{
	this->cam_far = cam_far;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(this->fov),
		this->w_width / this->w_height,
		this->cam_near,
		this->cam_far
	);
}

void TestCam::setFOV(float fov)
{
	this->fov = fov;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(this->fov),
		this->w_width / this->w_height,
		this->cam_near,
		this->cam_far
	);
}

float TestCam::getFOV()
{
	return this->fov;
}

float TestCam::getNear()
{
	return this->cam_near;
}

float TestCam::getFar()
{
	return this->cam_far;
}

void TestCam::setWidth(float w)
{
	this->w_width = w;
}

void TestCam::setHeight(float h)
{
	this->w_height = h;
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
