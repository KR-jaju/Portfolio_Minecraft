#include "pch.h"
#include "Window.h"

Window::Window(HINSTANCE instance, int nCmdShow, std::wstring const& class_name, std::wstring const& title)
{
	this->registerClass(instance, class_name);
	this->handle = this->initialize(instance, nCmdShow, class_name, title);
}

void	Window::setCallback(std::unique_ptr<WindowCallback>&& callback) {
	SetWindowLongPtr(this->handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(callback.get()));
	this->callback = std::move(callback);
}

HWND	Window::getHandle() const
{
	return (this->handle);
}

static LRESULT	windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowCallback* callback = reinterpret_cast<WindowCallback*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (callback == nullptr)
		return DefWindowProc(hWnd, msg, wParam, lParam);
	return callback->onEvent(hWnd, msg, wParam, lParam);
}

ATOM	Window::registerClass(HINSTANCE instance, std::wstring const& class_name) {
	LPCWSTR window_class = reinterpret_cast<LPCWSTR>(class_name.c_str());
	WNDCLASSEXW wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = windowProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = window_class;
	wcex.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(IDI_APPLICATION));

	return RegisterClassExW(&wcex);
}

HWND	Window::initialize(HINSTANCE instance, int nCmdShow, std::wstring const& class_name, std::wstring const& title)
{
	LPCWSTR	window_class = reinterpret_cast<LPCWSTR>(class_name.c_str());
	LPCWSTR window_title = reinterpret_cast<LPCWSTR>(title.c_str());
	RECT rect = { 0, 0, 400, 400 };
	HWND handle = nullptr;

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	
	handle = CreateWindow(window_class, window_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, instance, nullptr);
	if (handle == nullptr)
	{
		throw std::runtime_error("Failed to create window");
	}
	::ShowWindow(handle, nCmdShow);
	::UpdateWindow(handle);
	return (handle);
}