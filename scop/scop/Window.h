#pragma once
#include "framework.h"
#include "WindowCallback.h"

class Window {
public:
	Window(HINSTANCE instance, int nCmdShow, std::wstring const& class_name, std::wstring const& title = L"Default Window");
	Window(Window const&) = delete;
	Window& operator=(Window const&) = delete;
	void	setCallback(std::unique_ptr<WindowCallback>&& callback);
	HWND	getHandle() const;
private:
	ATOM	registerClass(HINSTANCE instance, std::wstring const& class_name);
	HWND	initialize(HINSTANCE instance, int nCmdShow, std::wstring const& class_name, std::wstring const& title);

	HWND							handle;
	std::unique_ptr<WindowCallback>	callback;
};