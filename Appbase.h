#pragma once

#include <d3dcompiler.h> // for compile shaders

//#include "Image.h"
#include "framework.h"
#include "Circle.h"
#include "Raytracer.h"

struct Vertex
{
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec2 uv;
};

class Image
{
public:
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<glm::vec4> pixels;

    void ReadFromFile(const char* filename);
};

class Appbase
{
public:
	Appbase(HWND hWnd, int width, int height)
	{
        m_image.ReadFromFile("image.jpg");
        //m_circle = std::make_unique<Circle>(Circle({ 0.0f, 0.0f }, 0.5f, { 0.0f, 0.5f, 1.0f, 1.0f }));
        m_raytracer = std::make_unique<Raytracer>(width, height);

		Initialize(hWnd, width, height);
	}

    ~Appbase()
    {
    }


    void InitShaders()
    {
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

        if (FAILED(D3DCompileFromFile(L"ImageVS.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
        {
            if (errorBlob) {
                std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
            }
        }
        
        if (FAILED(D3DCompileFromFile(L"ImagePS.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
        {
            if (errorBlob) {
                std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
            }
        }

        m_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, m_imageVertexShader.GetAddressOf());
        m_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, m_imagePixelShader.GetAddressOf());

        /////////////////////////////////
        // create input layout objects //
        /////////////////////////////////
        D3D11_INPUT_ELEMENT_DESC inputLayout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        m_device->CreateInputLayout(inputLayout, 3, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), m_inputLayout.GetAddressOf());
        m_context->IASetInputLayout(m_inputLayout.Get());
    }

	void Initialize(HWND hWnd, int width, int height)
	{
        this->width = width;
        this->height = height;

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
            std::cout << "Failed: CreateDeviceAndSwapChain()" << std::endl;
            return;
        }

        //////////////////////////
        // create render target //
        //////////////////////////
        m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_backBuffer.GetAddressOf()));

        hr = m_device->CreateRenderTargetView(
            m_backBuffer.Get(),
            NULL,
            m_RTV.GetAddressOf()
        );

        if (FAILED(hr))
        {
            std::cout << "Failed: CreateRenderTargetView()" << std::endl;
            return;
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
        InitShaders();

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

        hr = m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());

        if (FAILED(hr))
        {
            std::cout << "Failed: CreateSamplerState()" << std::endl;
            return;
        }

        ////////////////////
        // create texture //
        ////////////////////
        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DYNAMIC;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        textureDesc.MiscFlags = 0;

        hr = m_device->CreateTexture2D(&textureDesc, NULL, m_canvasTexture.GetAddressOf());

        if (FAILED(hr))
        {
            std::cout << "Failed: CreateTexture2D()" << std::endl;
            return;
        }
        else
        {
            m_device->CreateShaderResourceView(m_canvasTexture.Get(), nullptr, m_canvasSRV.GetAddressOf());
            
            //D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
            //RTVDesc.Format = textureDesc.Format;
            //RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            //RTVDesc.Texture2D.MipSlice = 0;
            //
            //m_device->CreateRenderTargetView(m_canvasTexture.Get(), &RTVDesc, m_canvasRTV.GetAddressOf());
        }
        
        //////////////////////////
        // create image texture //
        //////////////////////////
        D3D11_TEXTURE2D_DESC imageTextureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        imageTextureDesc.Width = width;
        imageTextureDesc.Height = height;
        imageTextureDesc.MipLevels = 1;
        imageTextureDesc.ArraySize = 1;
        imageTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        imageTextureDesc.SampleDesc.Count = 1;
        imageTextureDesc.SampleDesc.Quality = 0;
        imageTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
        imageTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        imageTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        imageTextureDesc.MiscFlags = 0;

        hr = m_device->CreateTexture2D(&imageTextureDesc, NULL, m_imageTexture.GetAddressOf());

        if (FAILED(hr))
        {
            std::cout << "Failed: CreateTexture2D()" << std::endl;
            return;
        }
        else
        {
            m_device->CreateShaderResourceView(m_imageTexture.Get(), nullptr, m_imageSRV.GetAddressOf());

            //D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
            //RTVDesc.Format = textureDesc.Format;
            //RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            //RTVDesc.Texture2D.MipSlice = 0;
            //
            //m_device->CreateRenderTargetView(m_imageTexture.Get(), &RTVDesc, m_imageRTV.GetAddressOf());
        }

        D3D11_MAPPED_SUBRESOURCE ms;
        hr = m_context->Map(m_imageTexture.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, m_image.pixels.data(), m_image.pixels.size() * sizeof(glm::vec4));
        m_context->Unmap(m_imageTexture.Get(), NULL);

        //////////////////////////
        // create vertex buffer //
        //////////////////////////
        {
            const std::vector<Vertex> vertices =
            {
                { { -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
                { { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                { {  1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
                { {  1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            };
        
            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = UINT(sizeof(Vertex) * vertices.size());             // size is the VERTEX struct * 3
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = sizeof(Vertex);
        
            D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
            vertexBufferData.pSysMem = vertices.data();
            vertexBufferData.SysMemPitch = 0;
            vertexBufferData.SysMemSlicePitch = 0;
        
            const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer0.GetAddressOf());

            if (FAILED(hr)) {
                std::cout << "Failed: CreateBuffer()_Vertex0" << std::endl;
                return;
            }
        }

        {
            const std::vector<Vertex> vertices =
            {
                { { -0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
                { { -0.8f,  0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
                { {  0.8f,  0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
                { {  0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
            };

            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = UINT(sizeof(Vertex) * vertices.size());             // size is the VERTEX struct * 3
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = sizeof(Vertex);

            D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
            vertexBufferData.pSysMem = vertices.data();
            vertexBufferData.SysMemPitch = 0;
            vertexBufferData.SysMemSlicePitch = 0;

            const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer1.GetAddressOf());

            if (FAILED(hr)) {
                std::cout << "Failed: CreateBuffer()_Vertex1" << std::endl;
                return;
            };
        }

        /////////////////////////
        // create index buffer //
        /////////////////////////
        {
            const std::vector<uint16_t> indices =
            {
                0, 1, 2,
                0, 2, 3,
            };
        
            indexCount = indices.size();
        
            D3D11_BUFFER_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(bufferDesc));
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;        // use as a index buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = sizeof(uint16_t);
        
            D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
            indexBufferData.pSysMem = indices.data();
            indexBufferData.SysMemPitch = 0;
            indexBufferData.SysMemSlicePitch = 0;
        
            const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());

            if (FAILED(hr)) {
                std::cout << "Failed: CreateBuffer()_Index" << std::endl;
                return;
            };
        }

        ///////////////////////////////////
        // create vertex constant buffer //
        ///////////////////////////////////
        {
            D3D11_BUFFER_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(bufferDesc));
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = sizeof(PSConstantBufferData);
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;     // use as a constant buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };
            constantBufferData.pSysMem = &m_pixelConstantBufferData;
            constantBufferData.SysMemPitch = 0;
            constantBufferData.SysMemSlicePitch = 0;

            hr = m_device->CreateBuffer(&bufferDesc, &constantBufferData, m_vertexConstantBuffer.GetAddressOf());

            if (FAILED(hr)) {
                std::cout << "Failed: CreateBuffer()_VSConstant" << std::endl;
                return;
            };
        }

        //////////////////////////////////
        // create pixel constant buffer //
        //////////////////////////////////
        {
            D3D11_BUFFER_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(bufferDesc));
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = sizeof(PSConstantBufferData);
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;     // use as a constant buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = 0;
        
            D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };
            constantBufferData.pSysMem = &m_pixelConstantBufferData;
            constantBufferData.SysMemPitch = 0;
            constantBufferData.SysMemSlicePitch = 0;
        
            hr = m_device->CreateBuffer(&bufferDesc, &constantBufferData, m_pixelConstantBuffer.GetAddressOf());

            if (FAILED(hr)) {
                std::cout << "Failed: CreateBuffer()_PSConstant" << std::endl;
                return;
            };
        }
    }

    void Update()
    {
        std::vector<glm::vec4> pixels(width * height, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

        m_raytracer->Render(pixels);

        ///////////////////
        // update canvas //
        ///////////////////
        D3D11_MAPPED_SUBRESOURCE ms;
        m_context->Map(m_canvasTexture.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        //memcpy(ms.pData, m_image.pixels.data(), m_image.pixels.size() * sizeof(glm::vec4));
        memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(glm::vec4));
        m_context->Unmap(m_canvasTexture.Get(), NULL);

        //////////////////////////////////
        // update pixel constant buffer //
        //////////////////////////////////
        //m_context->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &m_constantBufferData, 0, 0);
        m_context->Map(m_pixelConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, &m_pixelConstantBufferData, sizeof(PSConstantBufferData));
        m_context->Unmap(m_pixelConstantBuffer.Get(), NULL);

        ////////////////////////
        // update view matrix //
        ////////////////////////
        //m_vertexConstantBufferData.view = XMMatrixLookToLH(m_viewPos, m_viewDir, m_viewUp);
        //m_vertexConstantBufferData.view = m_vertexConstantBufferData.view.Transpose();
        //
        //m_context->Map(m_vertexConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        //memcpy(ms.pData, &m_vertexConstantBufferData, sizeof(VSConstantBufferData));
        //m_context->Unmap(m_vertexConstantBuffer.Get(), NULL);
    }

    void Render()
    {
        m_context->RSSetViewports(1, &m_viewport);
        m_context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), nullptr);
        m_context->ClearRenderTargetView(m_RTV.Get(), initColor);

        m_context->VSSetShader(m_imageVertexShader.Get(), 0, 0);
        m_context->PSSetShader(m_imagePixelShader.Get(), 0, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer0.GetAddressOf(), &stride, &offset);
        m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
        m_context->PSSetShaderResources(0, 1, m_imageSRV.GetAddressOf());
        m_context->PSSetConstantBuffers(0, 1, m_pixelConstantBuffer.GetAddressOf());
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->DrawIndexed(indexCount, 0, 0);

        //m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
        m_context->PSSetShader(m_pixelShader.Get(), 0, 0);
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer1.GetAddressOf(), &stride, &offset);
        m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());
        m_context->DrawIndexed(indexCount, 0, 0);
    }

    void Clean()
    {

    }

public:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11Texture2D> m_backBuffer;

    ComPtr<ID3D11RenderTargetView> m_RTV;
    ComPtr<ID3D11ShaderResourceView> m_imageSRV;
    ComPtr<ID3D11RenderTargetView> m_imageRTV;
    ComPtr<ID3D11ShaderResourceView> m_canvasSRV;
    ComPtr<ID3D11RenderTargetView> m_canvasRTV;
    ComPtr<ID3D11SamplerState> m_samplerState;
    D3D11_VIEWPORT m_viewport;

    ComPtr<ID3D11VertexShader> m_imageVertexShader;
    ComPtr<ID3D11PixelShader> m_imagePixelShader;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;

    ComPtr<ID3D11Buffer> m_vertexBuffer0;
    ComPtr<ID3D11Buffer> m_vertexBuffer1;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    UINT indexCount;
    ComPtr<ID3D11Buffer> m_vertexConstantBuffer;
    ComPtr<ID3D11Buffer> m_pixelConstantBuffer;

    Vector3 m_viewPos = { 0.0f, 0.0f, -1.0f };
    Vector3 m_viewDir = { 0.0f, 0.0f, 1.0f };
    Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };

    ComPtr<ID3D11Texture2D> m_canvasTexture;
    ComPtr<ID3D11Texture2D> m_imageTexture;

    struct VSConstantBufferData
    {
        Matrix view;
    };

    VSConstantBufferData m_vertexConstantBufferData;

    struct PSConstantBufferData
    {
        bool textureOnOff = false;
        bool dummy0;
        char dummy1;
        int  dummy2;
        double dummy3;
    };

    PSConstantBufferData m_pixelConstantBufferData;

    float initColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

    Image m_image;
    std::unique_ptr<Circle> m_circle;
    std::unique_ptr<Raytracer> m_raytracer;

    int width, height;
};

