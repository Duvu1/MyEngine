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

    void Clean()
    {
    }

public:
    HWND m_hWnd;

    int m_width, m_height;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;

    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11Texture2D> m_backBuffer;
    ComPtr<ID3D11RenderTargetView> m_baseRTV;
    D3D11_VIEWPORT m_viewport;
    ComPtr<ID3D11SamplerState> m_samplerState;

    ComPtr<ID3D11ShaderResourceView> m_imageSRV0;
    ComPtr<ID3D11ShaderResourceView> m_imageSRV1;
    ComPtr<ID3D11RenderTargetView> m_imageRTV;

    ComPtr<ID3D11ShaderResourceView> m_canvasSRV;
    ComPtr<ID3D11RenderTargetView> m_canvasRTV;

    ComPtr<ID3D11VertexShader> m_imageVertexShader;
    ComPtr<ID3D11PixelShader> m_imagePixelShader;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;

    float m_initColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
};

