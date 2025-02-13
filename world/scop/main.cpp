
#include "pch.h"
#include "framework.h"
#include "scop.h"
#include "WindowCallback.h"
#include "Window.h"
#include "World.h"

void RegisterRawInput(HWND hwnd) {
    RAWINPUTDEVICE rid = {};

    rid.usUsagePage = 0x01;
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hwnd;
    rid.usUsage = 0x02; // 마우스 등록
    bool result = RegisterRawInputDevices(&rid, 1, sizeof(rid));
    assert(result);
    rid.usUsage = 0x06; // 키보드 등록
    result = RegisterRawInputDevices(&rid, 1, sizeof(rid));
    assert(result);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Window  window(hInstance, nCmdShow, L"WindowClassName", 800, 800);
    World game(window.getHandle(), 800, 800);
    MSG msg = {};
    //std::ios_base::sync_with_stdio(false);
    RegisterRawInput(window.getHandle());
    window.setCallback(&game);
    while (true)
    {
        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return (0);
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        game.update();
    }
    return (int)msg.wParam;
}