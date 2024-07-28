#include "ExampleApp.h"

using namespace std;

ExampleApp::ExampleApp()
    : Appbase()
{
    Image image;

    image.ReadFromFile("image0.jpg");
    m_images.push_back(image);

    image.ReadFromFile("image1.jpg");
    m_images.push_back(image);

    m_circle = std::make_unique<Circle>(Circle({ 0.0f, 0.0f }, 0.5f, { 0.0f, 0.5f, 1.0f, 1.0f }));
    m_raytracer = std::make_unique<Raytracer>(m_width, m_height);
}

ExampleApp::~ExampleApp()
{
}

bool ExampleApp::Initialize()
{
    HRESULT hr = S_OK;

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
        return false;
    }

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

    hr = m_device->CreateTexture2D(&textureDesc, NULL, m_canvasTexture.GetAddressOf());

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

    ComPtr<ID3D11Texture2D> imageTexture;
    hr = m_device->CreateTexture2D(&imageTextureDesc, NULL, imageTexture.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: CreateTexture2D()" << std::endl;
        return false;
    }
    else
    {
        ComPtr<ID3D11ShaderResourceView> SRV;
        hr = m_device->CreateShaderResourceView(imageTexture.Get(), nullptr, SRV.GetAddressOf());
        m_imageTextures.push_back(imageTexture);
        m_imageSRVs.push_back(SRV);

        //D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
        //RTVDesc.Format = textureDesc.Format;
        //RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        //RTVDesc.Texture2D.MipSlice = 0;
        //
        //m_device->CreateRenderTargetView(m_imageTexture.Get(), &RTVDesc, m_imageRTV.GetAddressOf());
    }

    hr = m_device->CreateTexture2D(&imageTextureDesc, NULL, imageTexture.GetAddressOf());

    if (FAILED(hr))
    {
        std::cout << "Failed: CreateTexture2D()" << std::endl;
        return false;
    }
    else
    {
        ComPtr<ID3D11ShaderResourceView> SRV;
        hr = m_device->CreateShaderResourceView(imageTexture.Get(), nullptr, SRV.GetAddressOf());
        m_imageTextures.push_back(imageTexture);
        m_imageSRVs.push_back(SRV);

        //D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
        //RTVDesc.Format = textureDesc.Format;
        //RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        //RTVDesc.Texture2D.MipSlice = 0;
        //
        //m_device->CreateRenderTargetView(m_imageTexture.Get(), &RTVDesc, m_imageRTV.GetAddressOf());
    }

    for (int i = 0; i < m_images.size(); i++)
    {
        D3D11_MAPPED_SUBRESOURCE ms;
        hr = m_context->Map(m_imageTextures[i].Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, m_images[i].pixels.data(), m_images[i].pixels.size() * sizeof(glm::vec4));
        m_context->Unmap(m_imageTextures[i].Get(), NULL);
    }

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

        const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer2D.GetAddressOf());

        if (FAILED(hr)) {
            std::cout << "Failed: CreateBuffer()_Vertex0" << std::endl;
            return false;
        }
    }
    
    ////////////////////////
    // initialize shaders //
    ////////////////////////
    InitShaders();

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

        const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer3D.GetAddressOf());

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

bool ExampleApp::InitShaders()
{
    ID3DBlob* vertexBlob = nullptr;
    ID3DBlob* pixelBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(L"VS2D.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
    {
        if (errorBlob) {
            std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
        }
    }

    if (FAILED(D3DCompileFromFile(L"PS2D.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
    {
        if (errorBlob) {
            std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
        }
    }

    m_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, m_vertexShader2D.GetAddressOf());
    m_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, m_pixelShader2D.GetAddressOf());


    if (FAILED(D3DCompileFromFile(L"VS3D.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
    {
        if (errorBlob) {
            std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
        }
    }

    if (FAILED(D3DCompileFromFile(L"PS3D.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
    {
        if (errorBlob) {
            std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
        }
    }

    m_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, m_vertexShader3D.GetAddressOf());
    m_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, m_pixelShader3D.GetAddressOf());

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

void ExampleApp::Update()
{
    if (m_dimension == 2)
    {
        // update pixel constant buffer
        UpdateBuffer(m_pixelConstantBuffer, m_pixelConstantBufferData);

        if (!m_textureOn)
        {
            std::vector<glm::vec4> pixels(m_width * m_height, glm::vec4(m_initColor[0], m_initColor[1], m_initColor[2], m_initColor[3]));

            for (int i = 0; i < m_width; i++)
                for (int j = 0; j < m_height; j++)
                {
                    float positionX = (float)(i - m_width / 2) / (m_height / 2);
                    float positionY = -(float)(j - m_height / 2) / (m_height / 2);

                    if (m_circle->IsInside(glm::vec2(positionX, positionY)))
                        pixels[i + m_width * j] = m_circle->color;
                }

            cout << m_circle->center.x << " " << m_circle->center.y << endl;

            // update canvas 
            D3D11_MAPPED_SUBRESOURCE ms;
            m_context->Map(m_canvasTexture.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(glm::vec4));
            m_context->Unmap(m_canvasTexture.Get(), NULL);
        }
    }
    else if (m_dimension == 3)
    {
        //m_raytracer->Render(pixels);

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
}

void ExampleApp::Render()
{
    m_context->RSSetViewports(1, &m_viewport);
    m_context->OMSetRenderTargets(1, m_baseRTV.GetAddressOf(), nullptr);
    m_context->ClearRenderTargetView(m_baseRTV.Get(), m_initColor);

    if (m_dimension == 2)
    {
        m_context->VSSetShader(m_vertexShader2D.Get(), 0, 0);
        m_context->PSSetShader(m_pixelShader2D.Get(), 0, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer2D.GetAddressOf(), &stride, &offset);
        m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

        if (m_textureOn == true)
        {
            //ID3D11ShaderResourceView* ary[2] = { m_imageSRV0.Get(), m_imageSRV1.Get() };
            m_context->PSSetShaderResources(0, m_imageSRVs.size(), m_imageSRVs[0].GetAddressOf());
        }
        else
        {
            std::vector<ID3D11ShaderResourceView*> nullSRV(m_imageSRVs.size(), nullptr);
            m_context->PSSetShaderResources(0, nullSRV.size(), nullSRV.data());
            m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());
        }

        m_context->PSSetConstantBuffers(0, 1, m_pixelConstantBuffer.GetAddressOf());
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->DrawIndexed(indexCount, 0, 0);
    }
    else if (m_dimension == 3)
    {

    }
    //m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
    //m_context->PSSetShader(m_pixelShader.Get(), 0, 0);
    //m_context->IASetVertexBuffers(0, 1, m_vertexBuffer1.GetAddressOf(), &stride, &offset);
    //m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());
    //m_context->DrawIndexed(indexCount, 0, 0);
}

void ExampleApp::UpdateGUI()
{
    if (ImGui::RadioButton("2D", m_dimension == 2))
    {
        m_dimension = 2;
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("3D", m_dimension == 3))
    {
        m_dimension = 3;
    }

    if (m_dimension == 2)
    {
        ImGui::Text("Image");
        ImGui::Checkbox("Image Texture", &m_textureOn);
        ImGui::SliderFloat("Threshold", &m_pixelConstantBufferData.threshold, 0.0f, 1.0f);

        if (!m_textureOn)
        {
            ImGui::Text("Circle");
            ImGui::SliderFloat2("Position", &m_circle->center.x, -m_width / m_height, m_width / m_height);
            ImGui::SliderFloat("Radius", &m_circle->radius, 0.0f, 3.0f);
            ImGui::SliderFloat3("Color", &m_circle->color.r, 0.0f, 1.0f);
            ImGui::SliderFloat3("Backround Color", m_initColor, 0.0f, 1.0f);
        }
    }
    else if (m_dimension == 3)
    {
        //ImGui::Text("My Position");
        //ImGui::SliderFloat("X", &example->m_raytracer->eyePos.x, -5.0f, 5.0f);
        //ImGui::SliderFloat("Y", &example->m_raytracer->eyePos.y, -5.0f, 5.0f);
        //ImGui::SliderFloat("Z", &example->m_raytracer->eyePos.z, -5.0f, 5.0f);

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
}