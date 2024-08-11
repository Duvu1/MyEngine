#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <d3dcompiler.h> // for compile shaders

#include "framework.h"

#include "MyEngine.h"

struct Vertex2D
{
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec2 uv;
};

struct Vertex3D
{
    Vector3 position;
    Vector3 color;
    Vector3 normal;
};

class Appbase
{
public:
    LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    Appbase();
    virtual ~Appbase();	

    bool Initialize();
    bool InitMainWindow();
    bool InitApp();
    bool InitGUI();

    int Run();

    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void UpdateGUI() = 0;

    virtual void KeyControl(int keyPressed) = 0;

    void CreateVertexShaderAndInputLayout(
        const std::wstring& filename,
        ComPtr<ID3D11VertexShader>& vertexShader,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElement,
        ComPtr<ID3D11InputLayout>& inputLayout
    );

    void CreatePixelShader(
        const std::wstring& filename,
        ComPtr<ID3D11PixelShader>& pixelShader
    );

    void CreateGeometryShader(
        const std::wstring& filename,
        ComPtr<ID3D11GeometryShader>& geometryShader
    );

    template <typename T>
    void UpdateBuffer(ComPtr<ID3D11Buffer>& buffer, const T& bufferData)
    {
        D3D11_MAPPED_SUBRESOURCE ms;
        m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, &bufferData, sizeof(bufferData));
        m_context->Unmap(buffer.Get(), NULL);
    }

    float GetAspectRatio() { return (float)m_width / m_height; };
    void Clean();

public:
    HWND m_hWnd;

    int m_width, m_height;
    FLOAT m_initColor[4] = { 0.4f, 0.4f, 0.4f, 1.0f };

    // init
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;

    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11Texture2D> m_backBuffer;
    ComPtr<ID3D11RenderTargetView> m_baseRTV;
    D3D11_VIEWPORT m_viewport;
    ComPtr<ID3D11RasterizerState> m_rasterizerState;

    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    // SRV, RTV
    std::vector<ComPtr<ID3D11ShaderResourceView>> m_imageSRVs;
    ComPtr<ID3D11RenderTargetView> m_imageRTV;

    ComPtr<ID3D11ShaderResourceView> m_canvasSRV;
    ComPtr<ID3D11RenderTargetView> m_canvasRTV;

    // shaders
    ComPtr<ID3D11InputLayout> m_inputLayout2D;
    ComPtr<ID3D11VertexShader> m_vertexShader2D;
    ComPtr<ID3D11PixelShader> m_pixelShader2D;

    ComPtr<ID3D11InputLayout> m_inputLayout3D;
    ComPtr<ID3D11VertexShader> m_vertexShader3D;
    ComPtr<ID3D11PixelShader> m_pixelShader3D;
    ComPtr<ID3D11GeometryShader> m_geometryShaderGrid;

    ComPtr<ID3D11InputLayout> m_inputLayoutGrid;
    ComPtr<ID3D11VertexShader> m_vertexShaderGrid;
    ComPtr<ID3D11PixelShader> m_pixelShaderGrid;

    ComPtr<ID3D11VertexShader> m_vertexShaderNormal3D;
    ComPtr<ID3D11PixelShader> m_pixelShaderNormal3D;

    // mouse
    int m_mousePosX;
    int m_mousePosY;
    glm::vec2 GetMousePos();
    void SetMousePos(int posX, int posY);

    // // click
    bool m_isLButtonPressed = false;
    bool m_isMButtonPressed = false;

    // // hold
    bool m_isDragging = false;

    // // scroll
    bool m_isScrolling = false;
    int m_mouseWheelDirection = 0;

    // keyboard
    bool m_key[256] = { false };
};

