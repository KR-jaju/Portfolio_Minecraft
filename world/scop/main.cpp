// scop.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "scop.h"


// test
#include "Terrain.h"
#include "time.h"
#include "TestCam.h"
// test 

#define MAX_LOADSTRING 100
HWND hWnd;

// test 용 전역변수
TestCam cam(800, 650, 60, 0.1, 1000);
vec3 dir;
int w_width = 800;
int w_height = 650;
bool lb_flag = false;
bool rb_flag = false;
bool fix_flag = true;
bool move_flag = true;
bool move_check = false;
// test

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    RECT client_rect;
    GetClientRect(hWnd, &client_rect);
    cam.setWidth(client_rect.right - 1);
    cam.setHeight(client_rect.bottom - 1);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCOP));

    // test code
    Terrain terrain(12, 12, hWnd, w_width, w_height, 1, 8); // 짝수 단위로만
    float h = terrain.getHeight(0.5, 0.5) + 0.5;
    cam.movePos(0.5, h, 0.5);
    cam.setDir(vec3(0, 0, 1));

    //cam.setDir(vec3(0, 0, 1));
    //cam.movePos(0, 15, -25);
    terrain.setSightChunk(1);
    // test code

    MSG msg = {};

    // 기본 메시지 루프입니다:
    cam.setCursorInClient(hWnd);
    move_check = true;
    int tp_idx = 0;
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            /*if (msg.message == WM_KEYDOWN) {
            }
            DispatchMessage(&msg);*/
        }
        else
        {
            if (lb_flag) {
                /*terrain.testClickLightBlock(cam.getPos(),
                    cam.getDir());*/
                terrain.putBlock(cam.getPos(), 
                    cam.getDir(), -3 + tp_idx);
                tp_idx++;
                if (tp_idx == 3)
                    tp_idx = 0;
                lb_flag = false;
            }
            if (rb_flag) {
                terrain.deleteBlock(cam.getPos(), cam.getDir());
                rb_flag = false;
            }
            cam.update();
            terrain.userPositionCheck(cam.getPos().x,
                cam.getPos().z);
            terrain.Render(
                cam.getViewProj().view,
                cam.getViewProj().proj,
                cam.getPos()
            );
        }
    }
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCOP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SCOP);
    wcex.lpszClassName  = L"SCOP";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
    RECT windowRect = { 0, 0, w_width, w_height };
    hWnd = CreateWindowW(L"SCOP", L"board", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, windowRect.right - windowRect.left, 
        windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            cout << "hello\n";
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        {
            if (wParam == 27) {
                // ESC 키가 눌렸을 때 프로그램 종료
                DestroyWindow(hWnd);
            }
            if (wParam == 13) {
                fix_flag ^= 1;
            }
        }
        break;
    case WM_LBUTTONDOWN:
        {
            lb_flag = true;
        }
        break;
    case WM_RBUTTONDOWN:
        {
            rb_flag = true;
        }
        break;
    case WM_MOUSEMOVE:
        {
        if (move_check && fix_flag) {
            RECT client_rect;
            GetClientRect(hWnd, &client_rect);
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);
            int centerX = (client_rect.right - client_rect.left) / 2;
            int centerY = (client_rect.bottom - client_rect.top) / 2;
            if (pt.x == centerX && pt.y == centerY)
            {
                return 0; // 움직임을 무시
            }
            cam.onMouseMove(hWnd, pt.x, pt.y);
            cam.setCursorInClient(hWnd);
        }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
