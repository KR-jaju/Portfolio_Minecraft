#pragma once
class TestCam
{
public:
	TestCam();
	~TestCam();
	void movePos(float x, float y, float z);
	void setDir(vec3 dir);
	void update();
	MVP getViewProj();
	vec3 getPos();
	vec3 getDir();

private:
	vec3 pos;
	vec3 dir;
	MVP mvp;
};

