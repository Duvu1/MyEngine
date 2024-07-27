#pragma once

#include "Appbase.h"
#include "Circle.h"
#include "Image.h"
#include "Raytracer.h"

class ExampleApp : public Appbase
{
public:
    void UpdateGUI()
    {
        ImGui::Text("Rectangle");
        ImGui::SliderFloat3("BG Color", m_initColor, 0.0f, 1.0f);
        ImGui::Checkbox("Image Texture", &m_textureOn);
        ImGui::SliderFloat("Threshold", &m_pixelConstantBufferData.threshold, 0.0f, 1.0f);
        //ImGui::Text("My Position");
        //ImGui::SliderFloat("X", &example->m_raytracer->eyePos.x, -5.0f, 5.0f);
        //ImGui::SliderFloat("Y", &example->m_raytracer->eyePos.y, -5.0f, 5.0f);
        //ImGui::SliderFloat("Z", &example->m_raytracer->eyePos.z, -5.0f, 5.0f);
        //ImGui::Text("Circle");
        //ImGui::SliderFloat2("Position", &example->m_circle->center.x, -width / height, width/ height);
        //ImGui::SliderFloat("Radius", &example->m_circle->radius, 0.0f, 3.0f);
        //ImGui::SliderFloat3("Color", &example->m_circle->color.r, 0.0f, 1.0f);
        //ImGui::Text("Sphere");
        //ImGui::SliderFloat3("Sphere Position", &example->m_raytracer->sphere->center.x, -width / height, width / height);
        //ImGui::SliderFloat("Radius", &example->m_raytracer->sphere->radius, 0.0f, 3.0f);
        //ImGui::SliderFloat("Reflection", &example->m_raytracer->sphere->reflection, 0.0f, 1.0f);
        //ImGui::SliderFloat3("Color", &example->m_raytracer->sphere->color.r, 0.0f, 1.0f);
        //ImGui::SliderFloat3("Ambient", &example->m_raytracer->sphere->ambient.x, 0.0f, 1.0f);
        //ImGui::SliderFloat3("Diffuse", &example->m_raytracer->sphere->diffuse.x, 0.0f, 1.0f);
        //ImGui::SliderFloat3("Specular", &example->m_raytracer->sphere->specular.x, 0.0f, 1.0f);
        //ImGui::Text("Light");
        //ImGui::SliderFloat3("Light Position", &example->m_raytracer->light.pos.x, -2.0f, 2.0f);
    }

	ExampleApp()
		: Appbase()
	{
        Image image;

        image.ReadFromFile("image0.jpg");
        m_image.push_back(image);

        image.ReadFromFile("image1.jpg");
        m_image.push_back(image);

        Initialize();
        InitShaders();

        //m_circle = std::make_unique<Circle>(Circle({ 0.0f, 0.0f }, 0.5f, { 0.0f, 0.5f, 1.0f, 1.0f }));
        m_raytracer = std::make_unique<Raytracer>(m_width, m_height);
	}

    bool Initialize()
    {
        ////////////////////
        // create texture //
        ////////////////////
        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Width = m_width;
        textureDesc.Height = m_height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DYNAMIC;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        textureDesc.MiscFlags = 0;

        HRESULT hr = m_device->CreateTexture2D(&textureDesc, NULL, m_canvasTexture.GetAddressOf());

        if (FAILED(hr))
        {
            std::cout << "Failed: CreateTexture2D()" << std::endl;
            return false;
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
        imageTextureDesc.Width = m_width;
        imageTextureDesc.Height = m_height;
        imageTextureDesc.MipLevels = 1;
        imageTextureDesc.ArraySize = 1;
        imageTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        imageTextureDesc.SampleDesc.Count = 1;
        imageTextureDesc.SampleDesc.Quality = 0;
        imageTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
        imageTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        imageTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        imageTextureDesc.MiscFlags = 0;

        hr = m_device->CreateTexture2D(&imageTextureDesc, NULL, m_imageTexture0.GetAddressOf());
        hr = m_device->CreateTexture2D(&imageTextureDesc, NULL, m_imageTexture1.GetAddressOf());

        if (FAILED(hr))
        {
            std::cout << "Failed: CreateTexture2D()" << std::endl;
            return false;
        }
        else
        {
            hr = m_device->CreateShaderResourceView(m_imageTexture0.Get(), nullptr, m_imageSRV0.GetAddressOf());
            hr = m_device->CreateShaderResourceView(m_imageTexture1.Get(), nullptr, m_imageSRV1.GetAddressOf());

            //D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
            //RTVDesc.Format = textureDesc.Format;
            //RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            //RTVDesc.Texture2D.MipSlice = 0;
            //
            //m_device->CreateRenderTargetView(m_imageTexture.Get(), &RTVDesc, m_imageRTV.GetAddressOf());
        }

        D3D11_MAPPED_SUBRESOURCE ms;
        hr = m_context->Map(m_imageTexture0.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, m_image[0].pixels.data(), m_image[0].pixels.size() * sizeof(glm::vec4));
        m_context->Unmap(m_imageTexture0.Get(), NULL);

        hr = m_context->Map(m_imageTexture1.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, m_image[1].pixels.data(), m_image[1].pixels.size() * sizeof(glm::vec4));
        m_context->Unmap(m_imageTexture1.Get(), NULL);

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
                return false;
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
                return false;
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

            indexCount = (UINT)indices.size();

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
                return false;
            };
        }

        ///////////////////////////////////
        // create vertex constant buffer //
        ///////////////////////////////////
        {
            D3D11_BUFFER_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(bufferDesc));
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bufferDesc.ByteWidth = sizeof(VSConstantBufferData);
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;     // use as a constant buffer
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };
            constantBufferData.pSysMem = &m_vertexConstantBufferData;
            constantBufferData.SysMemPitch = 0;
            constantBufferData.SysMemSlicePitch = 0;

            hr = m_device->CreateBuffer(&bufferDesc, &constantBufferData, m_vertexConstantBuffer.GetAddressOf());

            if (FAILED(hr)) {
                std::cout << "Failed: CreateBuffer()_VSConstant" << std::endl;
                return false;
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
                return false;
            };
        }

        return true;
    }

    bool InitShaders()
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

        return true;
    }

    void Update()
    {
        std::vector<glm::vec4> pixels(m_width * m_height, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
        //m_raytracer->Render(pixels);

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
        m_context->OMSetRenderTargets(1, m_baseRTV.GetAddressOf(), nullptr);
        m_context->ClearRenderTargetView(m_baseRTV.Get(), m_initColor);

        m_context->VSSetShader(m_imageVertexShader.Get(), 0, 0);
        m_context->PSSetShader(m_imagePixelShader.Get(), 0, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer0.GetAddressOf(), &stride, &offset);
        m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

        if (m_textureOn == true)
        {
            ID3D11ShaderResourceView* ary[2] = { m_imageSRV0.Get(), m_imageSRV1.Get() };
            m_context->PSSetShaderResources(0, 2, ary);
        }
        else
        {
            m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());
        }
        m_context->PSSetConstantBuffers(0, 1, m_pixelConstantBuffer.GetAddressOf());
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->DrawIndexed(indexCount, 0, 0);

        //m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
        //m_context->PSSetShader(m_pixelShader.Get(), 0, 0);
        //m_context->IASetVertexBuffers(0, 1, m_vertexBuffer1.GetAddressOf(), &stride, &offset);
        //m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());
        //m_context->DrawIndexed(indexCount, 0, 0);
    }

public:
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
    ComPtr<ID3D11Texture2D> m_imageTexture0;
    ComPtr<ID3D11Texture2D> m_imageTexture1;

    struct VSConstantBufferData
    {
        Matrix view;
    };

    struct PSConstantBufferData
    {
        float threshold = 0.5f;
        float dummy[3];
    };

    VSConstantBufferData m_vertexConstantBufferData;
    PSConstantBufferData m_pixelConstantBufferData;

    std::vector<Image> m_image;

    std::unique_ptr<Circle> m_circle;
    std::unique_ptr<Raytracer> m_raytracer;

    bool m_textureOn = false;
};

