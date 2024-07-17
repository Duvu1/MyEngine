#pragma once

#include "framework.h"
#include "d3dcompiler.h" // for compile shaders

class Appbase
{
public:
    struct Vec4
    {
        float v[4];
    };

    struct Vec2
    {
        float v[2];
    };

    struct Vertex
    {
        Vec4 pos;
        Vec2 uv;
    };

public:
	Appbase(HWND hWnd, int width, int height)
	{
		Initialize(hWnd, width, height);
	}

    ~Appbase()
    {

    }

	void Initialize(HWND hWnd, int width, int height)
	{
        //////////////////////
        // create swapchain //
        //////////////////////
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = width;
        swapChainDesc.BufferDesc.Height = height;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hWnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = TRUE;

        D3D_FEATURE_LEVEL FeatureLevels[] = { 
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0,
        };

        HRESULT hr = S_OK;
        D3D_FEATURE_LEVEL FeatureLevel;

        if (FAILED(D3D11CreateDeviceAndSwapChain(
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
        )
        ))
        {
            std::cout << "Failed: CreateDeviceAndSwapChain()" << std::endl;
        }

        //////////////////////////
        // create render target //
        //////////////////////////
        m_swapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            (LPVOID*)m_backBuffer.GetAddressOf()
        );

        if (FAILED(m_device->CreateRenderTargetView(
            m_backBuffer.Get(),
            NULL,
            m_RTV.GetAddressOf()
        )
        ))
        {
            std::cout << "Failed: CreateRenderTargetView()" << std::endl;
        }

        //////////////////
        // set viewport //
        //////////////////
        ZeroMemory(&m_viewport, sizeof(m_viewport));
        m_viewport.Width = width;
        m_viewport.Height = height;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0;
        m_viewport.TopLeftY = 0;

        m_context->RSSetViewports(1, &m_viewport);

        ////////////////////////
        // initialize shaders //
        ////////////////////////
        ID3DBlob* vertexBlob = nullptr;
        ID3DBlob* pixelBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        if (FAILED(D3DCompileFromFile(L"VS.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
        {
            if (errorBlob) {
                std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
            }
        }

        if (FAILED(D3DCompileFromFile(L"PS.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
        {
            if (errorBlob) {
                std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
            }
        }

        m_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, m_vertexShader.GetAddressOf());
        m_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, m_pixelShader.GetAddressOf());

        /////////////////////////////////
        // create input layout objects //
        /////////////////////////////////
        D3D11_INPUT_ELEMENT_DESC inputLayout[] = 
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        m_device->CreateInputLayout(inputLayout, 2, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), m_inputLayout.GetAddressOf());
        m_context->IASetInputLayout(m_inputLayout.Get());

        //////////////////////////
        // create sampler state //
        //////////////////////////
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // D3D11_FILTER_MIN_MAG_MIP_LINEAR
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());

        //////////////////////////
        // create vertex buffer //
        //////////////////////////
        {
            const std::vector<Vertex> vertices =
            {
                { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f,  1.0f } },
                { {  1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f,  1.0f } },
                { {  0.0f,  1.0f, 0.0f, 1.0f }, { 0.5f, -1.0f } },
            };

            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = UINT(sizeof(Vertex) * vertices.size());             // size is the VERTEX struct * 3
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.StructureByteStride = sizeof(Vertex);

            D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
            vertexBufferData.pSysMem = vertices.data();
            vertexBufferData.SysMemPitch = 0;
            vertexBufferData.SysMemSlicePitch = 0;

            const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());
            if (FAILED(hr)) {
                std::cout << "Failed: CreateBuffer()" << std::endl;
            };
        }

        /////////////////////////
        // create index buffer //
        /////////////////////////
        {
            const std::vector<uint16_t> indices =
            {
                2, 1, 0,
            };

            indexCount = indices.size();

            D3D11_BUFFER_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(bufferDesc));
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as a vertex buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.StructureByteStride = sizeof(uint16_t);

            D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
            indexBufferData.pSysMem = indices.data();
            indexBufferData.SysMemPitch = 0;
            indexBufferData.SysMemSlicePitch = 0;

            m_device->CreateBuffer(&bufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
        }
    }

    void Update()
    {

    }

    void Render()
    {
        float initColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };

        m_context->RSSetViewports(1, &m_viewport);
        m_context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), nullptr);
        m_context->ClearRenderTargetView(m_RTV.Get(), initColor);

        m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
        m_context->PSSetShader(m_pixelShader.Get(), 0, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
        m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
        //m_context->PSSetShaderResources(0, 1, )
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->DrawIndexed(indexCount, 0, 0);
    }

public:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11Texture2D> m_backBuffer;
    ComPtr<ID3D11RenderTargetView> m_RTV;
    ComPtr<ID3D11SamplerState> m_samplerState;
    D3D11_VIEWPORT m_viewport;

    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;

    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;

    int indexCount;
};

