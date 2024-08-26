// scop.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "scop.h"
#include "Game.h"

#include "Test2.h"
#include "WindowCallback.h"
#include "Window.h"

class WCallback : public WindowCallback {
public:
    LRESULT	onEvent(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) {
        if (msg == WM_SIZE) {
            std::cout << "Handler called!" << std::endl;
        }
        return DefWindowProc(handle, msg, w_param, l_param);
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    std::shared_ptr<d3d11::Device> device = make_shared<d3d11::Device>();
    Window  window(hInstance, nCmdShow, L"WindowClassName");
    MSG msg = {};
    Test2 handler(device, window.getHandle(), 400, 400); //Application
    std::cout << "start";
    window.setCallback(&handler);
    while (1) //msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        handler.update();
        handler.renderUV();
    }
    return (int) msg.wParam;
}
