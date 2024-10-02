#include "pch.h"
#include "InputSystem.h"

InputSystem::InputSystem(HWND hwnd)
{
	this->hwnd = hwnd;
	this->states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

InputSystem::~InputSystem()
{
}

void InputSystem::Update()
{
	HWND hwnd = ::GetActiveWindow();
	if (this->hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			this->states[key] = KEY_STATE::NONE;
		return;
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = this->states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = this->states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}

	::GetCursorPos(&this->mousePos);
	::ScreenToClient(this->hwnd, &this->mousePos);
}