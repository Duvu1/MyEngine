// MyEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MyEngine.h"
#include "Appbase.h"

#define MAX_LOADSTRING 100

// 전역 변수:
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYENGINE, szWindowClass, MAX_LOADSTRING);

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW; // 창 크기 조절 시 다시 그리기
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    const int width = 1280;
    const int height = 960;

    // 실제 해상도 설정
    RECT wrect = { 0, 0, width, height };
    AdjustWindowRect(&wrect, WS_OVERLAPPEDWINDOW, FALSE);

    // 애플리케이션 초기화를 수행합니다:
    HWND hWnd = CreateWindowW(
        wcex.lpszClassName,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        0,
        wrect.right - wrect.left,
        wrect.bottom - wrect.top,
        nullptr,
        nullptr,
        wcex.hInstance,
        nullptr
    );

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    auto example = std::make_unique<Appbase>(hWnd, width, height);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYENGINE));

    // 기본 메시지 루프입니다:
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            example->Update();
            example->Render();

            example->m_swapChain->Present(1, 0);
        }
    }

    return (int) msg.wParam;
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
    case WM_CREATE:
        AllocConsole();
        SetConsoleTitle(TEXT("MyEngine"));
        _tfreopen(_T("CONOUT$"), _T("w"), stdout);
        _tfreopen(_T("CONERR$"), _T("w"), stderr);
        _tsetlocale(LC_ALL, _T(""));
        break;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu 
            return 0;
        break;
    case WM_MOUSEMOVE:
        std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
        break;
    case WM_LBUTTONUP:
        std::cout << "WM_LBUTTONUP Left mouse button" << std::endl;
        break;
    case WM_RBUTTONUP:
        std::cout << "WM_RBUTTONUP Right mouse button" << std::endl;
        break;
    case WM_KEYDOWN:
        std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

