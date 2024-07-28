#include "Appbase.h"

using namespace std;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Appbase* g_appBase = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_appBase->MsgProc(hWnd, message, wParam, lParam);
}

Appbase::Appbase()
    : m_width(1280)
    , m_height(960)
    , m_hWnd(NULL)
{
    g_appBase = this;

    Initialize();
}

Appbase::~Appbase()
{
    g_appBase = nullptr;

    // ImGui shutdown
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool Appbase::Initialize()
{
    if (!InitMainWindow())
    {
        cout << "Failed: InitMainWindow()" << endl;
        return false;
    }

    if (!InitApp())
    {
        cout << "Failed: InitApp()" << endl;
        return false;
    }

    if (!InitGUI())
    {
        cout << "Failed: InitGUI()" << endl;
        return false;
    }

    return true;
}

bool Appbase::InitMainWindow()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW; // 창 크기 조절 시 다시 그리기
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MYENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"MyEngine";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // 실제 해상도 설정
    RECT wrect = { 0, 0, m_width, m_height };
    AdjustWindowRect(&wrect, WS_OVERLAPPEDWINDOW, FALSE);

    // 애플리케이션 초기화를 수행합니다:
    m_hWnd = CreateWindowW(
        wcex.lpszClassName,
        L"MyEngine",
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

    if (!m_hWnd)
    {
        cout << "Failed: CreateWindow()" << endl;
        return false;
    }

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hWnd);

    return true;
}

bool Appbase::InitApp()
{
    HRESULT hr = S_OK;
    
    //////////////////////
    // create swapchain //
    //////////////////////
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = m_width;
    swapChainDesc.BufferDesc.Height = m_height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = m_hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL FeatureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL FeatureLevel;

    hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &FeatureLevel,
        m_context.GetAddressOf()
    );

    if (FAILED(hr))
    {
        cout << "Failed: CreateDeviceAndSwapChain()" << endl;
        return false;
    }

    //////////////////////////
    // create render target //
    //////////////////////////
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_backBuffer.GetAddressOf()));

    hr = m_device->CreateRenderTargetView(
        m_backBuffer.Get(),
        NULL,
        m_baseRTV.GetAddressOf()
    );

    if (FAILED(hr))
    {
        cout << "Failed: CreateRenderTargetView()" << endl;
        return false;
    }

    //////////////////
    // set viewport //
    //////////////////
    ZeroMemory(&m_viewport, sizeof(m_viewport));
    m_viewport.Width = (FLOAT)m_width;
    m_viewport.Height = (FLOAT)m_height;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;

    m_context->RSSetViewports(1, &m_viewport);

    return true;
}

bool Appbase::InitGUI()
{
    // Setup Dear Imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(m_hWnd))
    {
        cout << "Failed: ImGui_ImpWin32_Init()" << endl;
        return false;
    }

    if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get()))
    {
        cout << "Failed: ImGui_ImplDX11_Init()" << endl;
        return false;
    }

    return true;
}

LRESULT Appbase::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

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

int Appbase::Run()
{
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
            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();

            ImGui::NewFrame();
            ImGui::Begin("Toolbar");
            
            UpdateGUI();

            ImGui::End();
            ImGui::Render();

            // My rendering
            Update();
            Render();

            // ImGui rendering
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            m_swapChain->Present(1, 0);
        }
    }

    return 0;
}
