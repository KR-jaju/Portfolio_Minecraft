#include "pch.h"
#include "InputRegistry.h"

InputRegistry::InputRegistry(HWND handle, int width, int height)
	: handle(handle),
	width(width), height(height),
	is_locked(true),
	last_mouse_x(0), last_mouse_y(0),
	last_mouse_dx(0), last_mouse_dy(0),
	mouse_dx(0), mouse_dy(0)
{
	this->setMousePosition(0, 0);
}

void	InputRegistry::setMouseLock(bool lock)
{
	this->is_locked = lock;
}


void	InputRegistry::setMousePosition(float x, float y)
{
	POINT mouse = {
		(x * 0.5f + 0.5f)* this->width,
		(y * 0.5f + 0.5f)* this->height
	};

	this->last_mouse_x = mouse.x;
	this->last_mouse_y = mouse.y;
	::ClientToScreen(this->handle, &mouse);
	::SetCursorPos(mouse.x, mouse.y);
}


std::pair<float, float> InputRegistry::getMousePosition() const
{
	POINT mouse;

	::GetCursorPos(&mouse);
	::ScreenToClient(this->handle, &mouse);

	return {
		(mouse.x / this->width - 0.5f) * 2.0f,
		(mouse.y / this->height - 0.5f) * 2.0f
	};
}

std::pair<float, float>	InputRegistry::getMouseDelta() const
{
	return {
		static_cast<float>(this->last_mouse_dx) / this->width * 2.0f,
		static_cast<float>(this->last_mouse_dy) / this->width * 2.0f // 의도한 것임! 상하/좌우의 스케일이 동일함
	};
}

std::pair<float, float> InputRegistry::getMovementInput() const
{
	return {
		this->horizontal,
		this->vertical
	};
}

void	InputRegistry::onInputEvent(LPARAM l_param)
{
	RAWINPUT raw;
	UINT dataSize = sizeof(RAWINPUT);

	GetRawInputData((HRAWINPUT)l_param, RID_INPUT, &raw, &dataSize, sizeof(RAWINPUTHEADER));

	if (raw.header.dwType == RIM_TYPEMOUSE) {
		RAWMOUSE& mouse = raw.data.mouse;
		int deltaX = mouse.lLastX;
		int deltaY = mouse.lLastY;

		this->mouse_dx += deltaX;
		this->mouse_dy += deltaY;
	}
	else if (raw.header.dwType == RIM_TYPEKEYBOARD) {
		RAWKEYBOARD& keyboard = raw.data.keyboard;
		USHORT key = keyboard.VKey;
		USHORT flags = keyboard.Flags; // 키가 떨어졌는가 붙었는가
		bool is_key_down = (flags & RI_KEY_BREAK) == 0;
		bool was_key_down = this->key_down[key];

		if (is_key_down && !was_key_down) // 새로 눌린 키라면
			this->key_down[key] = true;
		else if (!is_key_down && was_key_down) // 새로 떼진 키라면
			this->key_up[key] = true;
		this->key_pressed[key] = is_key_down; // 현재 눌려있는지
	}
	
}


void	InputRegistry::update()
{
	if (this->is_locked && ::GetForegroundWindow() == this->handle)
	{
		this->setMousePosition(0, 0);
	}
	this->last_mouse_dx = this->mouse_dx;
	this->last_mouse_dy = this->mouse_dy;
	this->mouse_dx = 0;
	this->mouse_dy = 0;

	this->polled_key_down.swap(this->key_down);
	this->polled_key_up.swap(this->key_up);
	this->polled_key_pressed = this->key_pressed;

	this->key_down.clear();
	this->key_up.clear();

	this->horizontal = this->getKeyPressed('A') - this->getKeyPressed('D');
	this->vertical = this->getKeyPressed('W') - this->getKeyPressed('S');
}


bool	InputRegistry::getKeyDown(USHORT key) const
{
	if (this->polled_key_down.count(key) == 0)
		return (false);
	return (this->polled_key_down.at(key));
}

bool	InputRegistry::getKeyUp(USHORT key) const
{
	if (this->polled_key_up.count(key) == 0)
		return (false);
	return (this->polled_key_up.at(key));
}

bool	InputRegistry::getKeyPressed(USHORT key) const
{
	if (this->polled_key_pressed.count(key) == 0)
		return (false);
	return (this->polled_key_pressed.at(key));
}
