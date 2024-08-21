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
}

Appbase::~Appbase()
{
    g_appBase = nullptr;

    // ImGui shutdown
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DestroyWindow(m_hWnd);
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
    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = S_OK;
    
    D3D_FEATURE_LEVEL FeatureLevels[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL FeatureLevel;

    ///////////////////
    // create device //
    ///////////////////
    hr = D3D11CreateDevice(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        createDeviceFlags,
        FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        m_device.GetAddressOf(),
        &FeatureLevel,
        m_context.GetAddressOf()
    );

    if (FAILED(hr))
    {
        cout << "Failed: D3D11CreateDevice()" << endl;
        return false;
    }

    // MSAA
    m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_numQualityLevels);

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
    if (m_numQualityLevels > 0)
    {
        swapChainDesc.SampleDesc.Count = 4;
        swapChainDesc.SampleDesc.Quality = m_numQualityLevels - 1;
    }
    else
    {
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
    }
    swapChainDesc.Windowed = TRUE;


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
    CreateRenderTargetView();

    //////////////////
    // set viewport //
    //////////////////
    SetViewport();

    /////////////////////////////
    // create rasterizer state // 
    /////////////////////////////
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false; // 삼각형 앞면의 정점이 시계 반대 방향으로 배치되어 있는지
    rasterizerDesc.DepthClipEnable = true; // nearZ, farZ 확인용

    hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());

    if (FAILED(hr))
    {
        cout << "Failed: CreateRasterizerState()" << endl;
        return false;
    }

    // to draw wireframe
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

    hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerStateWireframe.GetAddressOf());

    if (FAILED(hr))
    {
        cout << "Failed: CreateRasterizerState()" << endl;
        return false;
    }

    /////////////////////////
    // create depth buffer // depth 값을 저장하는 버퍼
    /////////////////////////
    CreateDepthBuffer();

    ////////////////////////////////
    // create depth stencil state //
    ////////////////////////////////

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
    if (FAILED(hr))
    {
        cout << "Failed: CreateDepthStencilState()" << endl;
        return false;
    }

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
    {
        AllocConsole();
        SetConsoleTitle(TEXT("MyEngine"));

        FILE* fp;
        fp = _tfreopen(_T("CONOUT$"), _T("w"), stdout);
        fp = _tfreopen(_T("CONIN$"), _T("r"), stdin);
        fp = _tfreopen(_T("CONERR$"), _T("w"), stderr);
        _tsetlocale(LC_ALL, _T(""));

        break;
    }
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu 
            return 0;
        break;
    case WM_SIZE:
        // resize swapchain
        if (m_swapChain)
        {
            m_width = int(LOWORD(lParam));
            m_height = int(HIWORD(lParam));

            m_baseRTV.Reset();
            m_swapChain->ResizeBuffers(
                0,
                (UINT)LOWORD(lParam),
                (UINT)HIWORD(lParam),
                DXGI_FORMAT_UNKNOWN,
                0
            );

            CreateRenderTargetView();
            CreateDepthBuffer();
            SetViewport();
        }
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        _tprintf(_T("Mouse %d %d\n"), x, y);
        //std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
        SetMousePos(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_LBUTTONDOWN:
        _tprintf(_T("WM_LBUTTONDOWN Left mouse button\n"));
        //std::cout << "WM_LBUTTONDOWN Left mouse button" << std::endl;
        m_isLButtonPressed = true;
        break;
    case WM_LBUTTONUP:
        _tprintf(_T("WM_LBUTTONUP Left mouse button\n"));
        //std::cout << "WM_LBUTTONUP Left mouse button" << std::endl;
        m_isDragging = false;
        m_isLButtonPressed = false;
        break;
    case WM_MBUTTONDOWN:
        m_isMButtonPressed = true;
        break;
    case WM_MBUTTONUP:
        m_isDragging = false;
        m_isMButtonPressed = false;
        break;
    case WM_MOUSEWHEEL:
    {
        int wheelValue = (int)wParam;
        _tprintf(_T("WM_MOUSEWHEEL %d\n"), wheelValue);
        //std::cout << "WM_MOUSEWHEEL " << (int)wParam << std::endl;
        m_isScrolling = true;

        if (wheelValue > 0)
            m_mouseWheelDirection = 1;
        else if (wheelValue < 0)
            m_mouseWheelDirection = -1;
        else
            m_mouseWheelDirection = 0;
        break;
    }
    case WM_RBUTTONUP:
        _tprintf(_T("WM_RBUTTONUP Left mouse button\n"));
        //std::cout << "WM_RBUTTONUP Right mouse button" << std::endl;
        break;
    case WM_KEYDOWN:
    {
        int keyValue = (int)wParam;
        _tprintf(_T("WM_KEYDOWN %d\n"), keyValue);
        //std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
        KeyControl(keyValue);
        break;
    }
    case WM_KEYUP:
    {
        int keyValue = (int)wParam;
        _tprintf(_T("WM_KEYUP %d\n"), keyValue);
        //std::cout << "WM_KEYUP " << (int)wParam << std::endl;
        //m_key[wParam] = false;
        break;
    }
    case WM_DESTROY:
        FreeConsole();
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
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

bool Appbase::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));

    HRESULT hr = m_device->CreateRenderTargetView(
        backBuffer.Get(),
        NULL,
        m_baseRTV.GetAddressOf()
    );

    if (FAILED(hr))
    {
        cout << "Failed: CreateRenderTargetView()" << endl;
        return false;
    }

    return true;
}

void Appbase::SetViewport()
{
    ZeroMemory(&m_viewport, sizeof(m_viewport));
    m_viewport.Width = (FLOAT)m_width;
    m_viewport.Height = (FLOAT)m_height;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;

    m_context->RSSetViewports(1, &m_viewport);
}

bool Appbase::CreateDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc; // buffer desc가 아니느 texture2d desc 사용
    ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
    depthStencilBufferDesc.Width = m_width;
    depthStencilBufferDesc.Height = m_height;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // depth 24bits, stencil 8bits
    if (m_numQualityLevels > 0)
    {
        depthStencilBufferDesc.SampleDesc.Count = 4;
        depthStencilBufferDesc.SampleDesc.Quality = m_numQualityLevels - 1;
    }
    else
    {
        depthStencilBufferDesc.SampleDesc.Count = 1;
        depthStencilBufferDesc.SampleDesc.Quality = 0;
    }
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.MiscFlags = 0;

    HRESULT hr = m_device->CreateTexture2D(&depthStencilBufferDesc, 0, m_depthStencilBuffer.GetAddressOf());

    if (FAILED(hr))
    {
        cout << "Failed: CreateTexture2D() while creating depth stencil buffer" << endl;
        return false;
    }

    hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, m_depthStencilView.GetAddressOf());

    if (FAILED(hr))
    {
        cout << "Failed: CreateDepthStencilView()" << endl;
        return false;
    }

    return true;
}

bool Appbase::CreateSamplerState(ComPtr<ID3D11SamplerState> samplerState)
{
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // D3D11_FILTER_MIN_MAG_MIP_LINEAR
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = m_device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: CreateSamplerState()" << std::endl;
        return false;
    }

    return true;
}

bool Appbase::CreateVertexShaderAndInputLayout(const std::wstring &filename, ComPtr<ID3D11VertexShader>& vertexShader,
    const vector<D3D11_INPUT_ELEMENT_DESC> &inputElement, ComPtr<ID3D11InputLayout> &inputLayout)
{
    HRESULT hr = S_OK;

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    hr = D3DCompileFromFile(filename.c_str(), 0, 0, "main", "vs_5_0", compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: D3DCompileFromFile()" << std::endl;
        return false;
    }

    hr = m_device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, vertexShader.GetAddressOf());
    
    if (FAILED(hr))
    {
        std::cout << "Failed: CreateVertexShader()" << std::endl;
        return false;
    }

    hr = m_device->CreateInputLayout(inputElement.data(), (UINT)inputElement.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), inputLayout.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: CreateInputLayout()" << std::endl;
        return false;
    }

    return true;
}

bool Appbase::CreatePixelShader(const std::wstring &filename, ComPtr<ID3D11PixelShader> &pixelShader)
{
    HRESULT hr = S_OK;

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    hr = D3DCompileFromFile(filename.c_str(), 0, 0, "main", "ps_5_0", compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: D3DCompileFromFile()" << std::endl;
        return false;
    }

    hr = m_device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, pixelShader.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: CreatePixelShader()" << std::endl;
        return false;
    }

    return true;
}

bool Appbase::CreateGeometryShader(const std::wstring& filename, ComPtr<ID3D11GeometryShader>& geometryShader)
{
    HRESULT hr = S_OK;

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    hr = D3DCompileFromFile(filename.c_str(), 0, 0, "main", "gs_5_0", compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: D3DCompileFromFile()" << std::endl;
        return false;
    }

    hr = m_device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, geometryShader.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: CreateGeometryShader()" << std::endl;
        return false;
    }

    return true;
}

bool Appbase::CreateIndexBuffer(ComPtr<ID3D11Buffer>& buffer, const std::vector<uint16_t>& indexData)
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indexData.size());
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;        // use as a index buffer
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA indexBufferData = { 0, };
    indexBufferData.pSysMem = indexData.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    HRESULT hr = m_device->CreateBuffer(&bufferDesc, &indexBufferData, buffer.GetAddressOf());

    if (FAILED(hr)) {
        std::cout << "Failed: CreateIndexBuffer()" << std::endl;
        return false;
    };

    return true;
}

void Appbase::SetMousePos(int posX, int posY)
{
    int clampPosX = posX;
    int clampPosY = posY;

    if (clampPosX > 60000)
        clampPosX = 0;
    else if (clampPosX > m_width)
        clampPosX = m_width;

    if (clampPosY > 60000)
        clampPosY = 0;
    else if (clampPosY > m_height)
        clampPosY = m_height;

    m_mousePosX = clampPosX - m_width / 2;
    m_mousePosY = -(clampPosY - m_height / 2);
}

Vector2 Appbase::GetMousePos()
{
    return Vector2(m_mousePosX, m_mousePosY);
}
