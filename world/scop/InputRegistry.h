#pragma once

#include <unordered_map>

class InputRegistry
{
public:
	InputRegistry(HWND handle, int width, int height);

	void	setMouseLock(bool lock);
	void	setMousePosition(float x, float y);

	std::pair<float, float> getMousePosition() const;

	std::pair<float, float>	getMouseDelta() const;
	std::pair<float, float> getMovementInput() const;

	bool	getKeyDown(USHORT key) const;
	bool	getKeyUp(USHORT key) const;
	bool	getKeyPressed(USHORT key) const;

	void	onInputEvent(LPARAM l_param);
	void	update();
private:
	HWND handle;

	int width;
	int height;

	bool	is_locked;

	long	last_mouse_x;
	long	last_mouse_y;
	int		last_mouse_dx;
	int		last_mouse_dy;
	long	mouse_dx;
	long	mouse_dy;

	//std::unordered_map<
	std::unordered_map<USHORT, bool>	key_down; // 처음 눌린 키들의 집합
	std::unordered_map<USHORT, bool>	key_up; // 처음 떼진 키들의 집합
	std::unordered_map<USHORT, bool>	key_pressed;

	std::unordered_map<USHORT, bool>	polled_key_down; // 처음 눌린 키들의 집합
	std::unordered_map<USHORT, bool>	polled_key_up; // 처음 떼진 키들의 집합
	std::unordered_map<USHORT, bool>	polled_key_pressed;

	float	horizontal;
	float	vertical;
};