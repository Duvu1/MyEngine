#include "ExampleApp.h"

#include <tuple>

using namespace std;

void MakeBox(vector<Vertex3D> &vertices, vector<uint16_t> &indices)
{
    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;

    const float scale = 1.0f;

    // À­¸é
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));

    // ¾Æ·§¸é
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));

    // ¾Õ¸é
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

    // µÞ¸é
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));

    // ¿ÞÂÊ
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));

    // ¿À¸¥ÂÊ
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vertex3D vertex;
        vertex.position = positions[i];
        vertex.color = colors[i];
        vertex.normal = normals[i];
        vertices.push_back(vertex);
    }

    vector<uint16_t> index = {
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
    };

    for (size_t i = 0; i < index.size(); i++)
        indices.push_back(index[i]);
}

ExampleApp::ExampleApp()
    : Appbase()
    , m_indexCount2D(0)
{
    Image image;

    image.ReadFromFile("image0.jpg");
    m_images.push_back(image);

    image.ReadFromFile("image1.jpg");
    m_images.push_back(image);

    m_model = std::make_unique<Model>();
    MakeBox(m_model->vertices, m_model->indices);
    //m_circle = std::make_unique<Circle>(Circle({ 0.0f, 0.0f }, 150.0f, { 0.0f, 1.0f, 1.0f, 1.0f }));
    //m_raytracer = std::make_unique<Raytracer>(m_width, m_height);
}

ExampleApp::~ExampleApp()
{
}

bool ExampleApp::Initialize()
{
    if (!Appbase::Initialize())
    {
        cout << "Failed: Initialize()" << endl;
        return false;
    }

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

    ////////////////////////
    // initialize shaders //
    ////////////////////////
    InitShaders();

    //////////////////////
    // create 2D object //
    //////////////////////
    // vertices
    {
        const std::vector<Vertex2D> vertices =
        {
            { { -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
            { {  1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        };

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
        bufferDesc.ByteWidth = UINT(sizeof(Vertex2D) * vertices.size());             // size is the VERTEX struct * 3
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = sizeof(Vertex2D);

        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
        vertexBufferData.pSysMem = vertices.data();
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;

        const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer2D.GetAddressOf());

        if (FAILED(hr)) {
            std::cout << "Failed: CreateBuffer()_Vertex2D" << std::endl;
            return false;
        }
    }

    // indices
    {
        const std::vector<uint16_t> indices =
        {
            0, 1, 2,
            0, 2, 3,
        };

        m_indexCount2D = indices.size();

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

        const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &indexBufferData, m_indexBuffer2D.GetAddressOf());

        if (FAILED(hr)) {
            std::cout << "Failed: CreateBuffer()_Index2D" << std::endl;
            return false;
        };
    }

    //////////////////////
    // create 3D object //
    //////////////////////
    // vertices
    {
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
        bufferDesc.ByteWidth = UINT(sizeof(Vertex3D) * m_model->vertices.size());             // size is the VERTEX struct * 3
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = sizeof(Vertex3D);

        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
        vertexBufferData.pSysMem = m_model->vertices.data();
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;

        const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer3D.GetAddressOf());

        if (FAILED(hr)) {
            std::cout << "Failed: CreateBuffer()_Vertex3D" << std::endl;
            return false;
        };
    }

    // indices
    {
        m_indexCount3D = m_model->indices.size();

        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
        bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * m_model->indices.size());
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;        // use as a index buffer
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = sizeof(uint16_t);

        D3D11_SUBRESOURCE_DATA indexBufferData = { 0, };
        indexBufferData.pSysMem = m_model->indices.data();
        indexBufferData.SysMemPitch = 0;
        indexBufferData.SysMemSlicePitch = 0;

        const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &indexBufferData, m_indexBuffer3D.GetAddressOf());

        if (FAILED(hr)) {
            std::cout << "Failed: CreateBuffer()_Index3D" << std::endl;
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

    ////////////////////////
    // create grid buffer //
    ////////////////////////
    {
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
        bufferDesc.ByteWidth = UINT(sizeof(Vector3) * m_grid.size());
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = sizeof(Vector3);

        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
        vertexBufferData.pSysMem = m_grid.data();
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;

        const HRESULT hr = m_device->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBufferGrid.GetAddressOf());

        if (FAILED(hr)) {
            std::cout << "Failed: CreateBuffer()_VertexGrid" << std::endl;
            return false;
        };

        m_indexCountGrid = m_grid.size();
    }

    return true;
}

bool ExampleApp::InitShaders()
{
    // 2D
    vector<D3D11_INPUT_ELEMENT_DESC> inputLayout2D =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    CreateVertexShaderAndInputLayout(L"VS2D.hlsl", m_vertexShader2D, inputLayout2D, m_inputLayout2D);
    CreatePixelShader(L"PS2D.hlsl", m_pixelShader2D);

    // 3D
    vector<D3D11_INPUT_ELEMENT_DESC> inputLayout3D =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    CreateVertexShaderAndInputLayout(L"VS3D.hlsl", m_vertexShader3D, inputLayout3D, m_inputLayout3D);
    CreatePixelShader(L"PS3D.hlsl", m_pixelShader3D);

    // grid
    vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutGrid =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    CreateVertexShaderAndInputLayout(L"VSGrid.hlsl", m_vertexShaderGrid, inputLayoutGrid, m_inputLayoutGrid);
    CreateGeometryShader(L"GSGrid.hlsl", m_geometryShaderGrid);
    CreatePixelShader(L"PSGrid.hlsl", m_pixelShaderGrid);

    // normal
    //if (FAILED(D3DCompileFromFile(L"NormalVS3D.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
    //{
    //    if (errorBlob) {
    //        std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
    //    }
    //}
    //
    //if (FAILED(D3DCompileFromFile(L"NormalPS3D.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
    //{
    //    if (errorBlob) {
    //        std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
    //    }
    //}
    //
    //hr = m_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, m_vertexShaderNormal3D.GetAddressOf());
    //hr = m_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, m_pixelShaderNormal3D.GetAddressOf());
    //
    //m_device->CreateInputLayout(inputLayout3D, 2, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), m_inputLayout3D.GetAddressOf());

    return true;
}

void ExampleApp::Update()
{

    if (m_dimension == 2)
    {
        std::vector<glm::vec4> pixels(m_width * m_height, glm::vec4(m_initColor[0], m_initColor[1], m_initColor[2], m_initColor[3]));

        // update pixel constant buffer
        UpdateBuffer(m_pixelConstantBuffer, m_pixelConstantBufferData);

        if (!m_textureOn)
        {
            //for (int i = 0; i < m_width; i++)
            //    for (int j = 0; j < m_height; j++)
            //    {
            //        //float positionScreenX = (float)(i - m_width / 2) / (m_height / 2);
            //        //float positionScreenY = -(float)(j - m_height / 2) / (m_height / 2);
            //        float positionScreenX = i - m_width / 2;
            //        float positionScreenY = -(j - m_height / 2);
            //
            //        if (m_raytracer->sphere->IsInside(glm::vec2(positionScreenX, positionScreenY)))
            //            pixels[i + m_width * j] = glm::vec4(m_raytracer->sphere->diffuse, 1.0);
            //    }
        
            // update canvas 
            D3D11_MAPPED_SUBRESOURCE ms;
            m_context->Map(m_canvasTexture.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(glm::vec4));
            m_context->Unmap(m_canvasTexture.Get(), NULL);
        }
    }
    else if (m_dimension == 3)
    {
        if (m_isMButtonPressed)
        {
            m_curMousePos = GetMousePos();

            if (!m_isDragging)
            {
                m_isDragging = true;
                m_prevMousePos = m_curMousePos;
            }
            else if (m_isDragging)
            {
                glm::vec2 translation = m_curMousePos - m_prevMousePos;
                translation.x *= -1.0f;
                translation /= 100.0f;

                //m_modelRotation.y += translation.x;
                //m_modelRotation.x += translation.y * cos(m_modelRotation.y);
                //m_modelRotation.z += translation.y * sin(m_modelRotation.y);

                m_viewAngle.x += translation.x;
                m_viewAngle.y += translation.y;

                m_viewPos.x = 5.0f * sin(m_viewAngle.x);
                //m_viewPos.y = -5.0f * sin(m_viewAngle.y);
                m_viewPos.z = -5.0f * cos(m_viewAngle.x);
                //m_viewPos.z += -5.0f * cos(m_viewAngle.y);
                
                m_prevMousePos = m_curMousePos;
            }
        }

        // update vertex constant buffer
        /////////////////////////
        // update model matrix //
        /////////////////////////
        m_vertexConstantBufferData.model = 
            Matrix::CreateScale(m_modelScale) *
            Matrix::CreateRotationX(m_modelRotation.x) *
            Matrix::CreateRotationY(m_modelRotation.y) *
            Matrix::CreateRotationZ(m_modelRotation.z) *
            Matrix::CreateTranslation(m_modelTranslation);
        m_vertexConstantBufferData.model = m_vertexConstantBufferData.model.Transpose();
        
        ////////////////////////
        // update view matrix //
        ////////////////////////
        m_vertexConstantBufferData.view = XMMatrixLookAtLH(m_viewPos, m_viewAt, m_viewUp);
        //m_vertexConstantBufferData.view = XMMatrixLookToLH(m_viewPos, m_viewDir, m_viewUp);
        m_vertexConstantBufferData.view = m_vertexConstantBufferData.view.Transpose();

        //////////////////////////////
        // update projection matrix //
        //////////////////////////////
        m_vertexConstantBufferData.projection = XMMatrixPerspectiveFovLH(
            XMConvertToRadians(m_fieldOfViewAngle), m_aspectRatio, m_nearZ, m_farZ);
        m_vertexConstantBufferData.projection = m_vertexConstantBufferData.projection.Transpose();

        UpdateBuffer(m_vertexConstantBuffer, m_vertexConstantBufferData);

        ///////////////////
        // update normal //
        ///////////////////
        m_normalVertexConstantBufferData.model = m_vertexConstantBufferData.model;
        m_normalVertexConstantBufferData.inverseTranspose = m_normalVertexConstantBufferData.model.Invert();
        m_normalVertexConstantBufferData.inverseTranspose = m_normalVertexConstantBufferData.inverseTranspose.Transpose();
        m_normalVertexConstantBufferData.view = m_vertexConstantBufferData.view;
        m_normalVertexConstantBufferData.projection = m_vertexConstantBufferData.projection;
        m_normalVertexConstantBufferData.scale = m_normalScale;

        //UpdateBuffer(m_normalVertexConstantBuffer, m_normalVertexConstantBufferData);

        /////////////////
        // update grid //
        /////////////////
        //UpdateBuffer(m_vertexConstantBuffer, m_vertexConstantBufferData);

        // update canvas  
        //D3D11_MAPPED_SUBRESOURCE ms;
        //m_context->Map(m_canvasTexture.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        //memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(glm::vec4));
        //m_context->Unmap(m_canvasTexture.Get(), NULL);
    }
}

void ExampleApp::Render()
{
    m_context->RSSetViewports(1, &m_viewport);
    m_context->OMSetRenderTargets(1, m_baseRTV.GetAddressOf(), m_depthStencilView.Get());
    m_context->ClearRenderTargetView(m_baseRTV.Get(), m_initColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
    m_context->RSSetState(m_rasterizerState.Get());

    if (m_dimension == 2)
    {
        m_context->IASetInputLayout(m_inputLayout2D.Get());
        m_context->VSSetShader(m_vertexShader2D.Get(), 0, 0);
        m_context->PSSetShader(m_pixelShader2D.Get(), 0, 0);

        UINT stride = sizeof(Vertex2D);
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer2D.GetAddressOf(), &stride, &offset);
        m_context->IASetIndexBuffer(m_indexBuffer2D.Get(), DXGI_FORMAT_R16_UINT, 0);

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

        m_context->DrawIndexed(m_indexCount2D, 0, 0);
    }
    else if (m_dimension == 3)
    {
        m_context->IASetInputLayout(m_inputLayout3D.Get());
        m_context->VSSetShader(m_vertexShader3D.Get(), 0, 0);
        m_context->PSSetShader(m_pixelShader3D.Get(), 0, 0);

        UINT stride = sizeof(Vertex3D);
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer3D.GetAddressOf(), &stride, &offset);
        m_context->IASetIndexBuffer(m_indexBuffer3D.Get(), DXGI_FORMAT_R16_UINT, 0);

        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

        std::vector<ID3D11ShaderResourceView*> nullSRV(m_imageSRVs.size(), nullptr);
        m_context->PSSetShaderResources(0, nullSRV.size(), nullSRV.data());
        m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());

        m_context->VSSetConstantBuffers(0, 1, m_vertexConstantBuffer.GetAddressOf());
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_context->DrawIndexed(m_indexCount3D, 0, 0);

        if (m_drawNormal)
        {
            m_context->VSSetShader(m_vertexShaderNormal3D.Get(), 0, 0);
            m_context->PSSetShader(m_pixelShaderNormal3D.Get(), 0, 0);

        
        }

        m_context->IASetInputLayout(m_inputLayoutGrid.Get());
        m_context->VSSetShader(m_vertexShaderGrid.Get(), 0, 0);
        m_context->GSSetShader(m_geometryShaderGrid.Get(), 0, 0);
        m_context->GSSetConstantBuffers(0, 1, m_vertexConstantBuffer.GetAddressOf());
        m_context->PSSetShader(m_pixelShaderGrid.Get(), 0, 0);

        stride = sizeof(Vector3);
        m_context->IASetVertexBuffers(0, 1, m_vertexBufferGrid.GetAddressOf(), &stride, &offset);
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        m_context->Draw(m_indexCountGrid, 0);

        m_context->GSSetShader(nullptr, 0, 0);
    }
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
    }
    else if (m_dimension == 3)
    {
        ImGui::Checkbox("Draw Normal", &m_drawNormal);
        ImGui::SliderFloat("Normal Scale", &m_normalScale, 0.0, 5.0);

        if (m_appState == APP_STATE::HOME)
        {
            ImGui::Text("Box");
            ImGui::SliderFloat3("Model Position", &m_modelTranslation.x, -2.0f, 2.0f);
            ImGui::SliderFloat3("View Position", &m_viewPos.x, -2.0f, 2.0f);
            ImGui::SliderFloat3("View Direction", &m_viewDir.x, -2.0f, 2.0f);
            ImGui::SliderFloat3("View At", &m_viewAt.x, -2.0f, 2.0f);
            ImGui::SliderFloat("Field Of View", &m_fieldOfViewAngle, 50.0f, 150.0f);
            ImGui::SliderFloat("Near Z", &m_nearZ, 0.01f, 10.0f);
            ImGui::SliderFloat("Far Z", &m_farZ, m_nearZ + 0.01f, 1000.0f);
            ImGui::SliderFloat("Aspect Ratio", &m_aspectRatio, 0.1f, 3.0f);
        }
        else if (m_appState == APP_STATE::EDIT_SCALE)
        {
            ImGui::Text("Scale");
            ImGui::SliderFloat3("Model Scale", &m_modelScale.x, 0.0f, 3.0f);
        }
        else if (m_appState == APP_STATE::EDIT_ROTATE)
        {
            ImGui::Text("Rotate");
            ImGui::SliderFloat3("Model Rotate", &m_modelRotation.x, -3.0f, 3.0f);
        }
    }   
}

void ExampleApp::KeyControl(int keyPressed)
{
    if (m_appState == APP_STATE::HOME)
    {
        if (keyPressed == 27)
            DestroyWindow(m_hWnd);
        else if (keyPressed == 'S')
            m_appState = APP_STATE::EDIT_SCALE;
        else if (keyPressed == 'R')
            m_appState = APP_STATE::EDIT_ROTATE;
        else if (keyPressed == 'T')
            m_appState = APP_STATE::EDIT_TRANSLATE;
    }
    else if (m_appState == APP_STATE::EDIT_SCALE)
    {
        if (keyPressed == 27)
            m_appState = APP_STATE::HOME;
    }
    else if (m_appState == APP_STATE::EDIT_ROTATE)
    {
        if (keyPressed == 27)
            m_appState = APP_STATE::HOME;
    }
    else if (m_appState == APP_STATE::EDIT_ROTATE)
    {
        if (keyPressed == 27)
            m_appState = APP_STATE::HOME;
    }

}
