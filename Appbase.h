#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <d3dcompiler.h> // for compile shaders

#include "framework.h"

#include "MyEngine.h"

struct Vertex
{
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec2 uv;
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

    template <typename T>
    void UpdateBuffer(ComPtr<ID3D11Buffer>& buffer, const T& bufferData)
    {
        D3D11_MAPPED_SUBRESOURCE ms;
        m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, &bufferData, sizeof(bufferData));
        m_context->Unmap(buffer.Get(), NULL);
    }
    void Clean();

    glm::vec2 GetMousePos();
    void SetMousePos(int posX, int posY);

public:
    HWND m_hWnd;

    int m_width, m_height;
    FLOAT m_initColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    //
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;

    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11Texture2D> m_backBuffer;
    ComPtr<ID3D11RenderTargetView> m_baseRTV;
    D3D11_VIEWPORT m_viewport;
    ComPtr<ID3D11SamplerState> m_samplerState;

    //
    std::vector<ComPtr<ID3D11ShaderResourceView>> m_imageSRVs;
    ComPtr<ID3D11RenderTargetView> m_imageRTV;

    ComPtr<ID3D11ShaderResourceView> m_canvasSRV;
    ComPtr<ID3D11RenderTargetView> m_canvasRTV;

    //
    ComPtr<ID3D11InputLayout> m_inputLayout;

    ComPtr<ID3D11VertexShader> m_vertexShader2D;
    ComPtr<ID3D11PixelShader> m_pixelShader2D;

    ComPtr<ID3D11VertexShader> m_vertexShader3D;
    ComPtr<ID3D11PixelShader> m_pixelShader3D;

    // 
    int m_mousePosX;
    int m_mousePosY;
    bool m_isLButtonPressed = false;

    bool m_isDragging = false;
};

