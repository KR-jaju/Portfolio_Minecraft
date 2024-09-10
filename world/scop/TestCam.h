#pragma once
class TestCam
{
public:
	TestCam(int width, int height);
	~TestCam();
	void movePos(float x, float y, float z);
	void setDir(vec3 dir);
	void onMouseMove(WPARAM btnState, int mouse_x, int mouse_y);
	void update();
	MVP getViewProj();
	vec3 getPos();
	vec3 getDir();
	void setCursorInClient(HWND hwnd, int c_x, int c_y);
private:
	int w_width;
	int w_height;
	vec3 pos;
	vec3 dir;
	MVP mvp;
};

