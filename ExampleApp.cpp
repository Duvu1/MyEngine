#include "ExampleApp.h"

#include <cmath>
#include <tuple>

#include "MakeMesh.cpp"

using namespace std;

ExampleApp::ExampleApp()
    : Appbase()
    , m_indexCount2D(0)
    , m_indexCount(0)
    , m_indexCountGrid(0)
{
    Image image;

    image.ReadFromFile("image0.jpg");
    m_images.push_back(image);

    image.ReadFromFile("image1.jpg");
    m_images.push_back(image);

    //MakeMesh::MakeNormal(model->GetVertices(),
    //           model->GetNormalVertices(), model->GetNormalIndices());
    //m_models.push_back(model);

    //shared_ptr<Mesh> cube = std::make_shared<Mesh>();
    //MakeBox(cube->GetVertices(), cube->GetIndices(), 10.0f);
    //m_models.push_back(cube);

    //shared_ptr<Mesh> sphere = std::make_shared<Mesh>();
    

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
    Appbase::CreateSamplerState(m_samplerState);

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
    const std::vector<Vertex2D> vertices =
    {
        { { -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
    };
    Appbase::CreateVertexBuffer(m_vertexBuffer2D, vertices);

    const std::vector<uint32_t> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };
    Appbase::CreateIndexBuffer(m_indexBuffer2D, indices);
    m_indexCount2D = indices.size();

    Appbase::CreateConstantBuffer(m_pixelConstantBuffer2D, m_pixelConstantBufferData2D);

    //////////////////////
    // create 3D object //
    //////////////////////
    vector<MeshData> meshes;
    meshes = MakeMesh::ReadFromFile("C:/Users/Duvu/source/repos/MyEngine/Assets/Meshes/", "monkey.obj");
    Appbase::CreateConstantBuffer(m_vertexConstantBuffer, m_vertexConstantBufferData);
    Appbase::CreateConstantBuffer(m_pixelConstantBuffer, m_pixelConstantBufferData);

    for (auto& meshData : meshes)
    {
        auto newMesh = std::make_shared<Mesh>();

        Appbase::CreateVertexBuffer(newMesh->vertexBuffer, meshData.vertices);
        Appbase::CreateIndexBuffer(newMesh->indexBuffer, meshData.indices);
        newMesh->indexCount = meshData.indices.size();

        //if (!meshData.textureFilename.empty())

        newMesh->vertexConstantBuffer = m_vertexConstantBuffer;
        newMesh->pixelConstantBuffer = m_pixelConstantBuffer;

        m_meshes.push_back(newMesh);
    }

    Appbase::CreateConstantBuffer(m_vertexConstantBufferFocus, m_vertexConstantBufferDataFocus);

    // normals
    m_normals = std::make_shared<Mesh>();
    std::vector<Vertex3D> normalVertices;
    std::vector<uint32_t> normalIndices;

    size_t offset = 0;
    for (auto& meshData : meshes)
    {
        for (size_t i = 0; i < meshData.vertices.size(); i++)
        {
            auto vertex = meshData.vertices[i];

            vertex.texcoord.x = 0.0f;
            normalVertices.push_back(vertex);
            normalIndices.push_back(uint32_t((i + offset) * 2));

            vertex.texcoord.x = 1.0f;
            normalVertices.push_back(vertex);
            normalIndices.push_back(uint32_t((i + offset) * 2 + 1));
        }
        offset += meshData.vertices.size();
    }

    Appbase::CreateVertexBuffer(m_normals->vertexBuffer, normalVertices);
    Appbase::CreateIndexBuffer(m_normals->indexBuffer, normalIndices);
    m_normals->indexCount = normalIndices.size();
    Appbase::CreateConstantBuffer(m_normals->vertexConstantBuffer, m_vertexConstantBufferDataNormal);
    
    /////////////////
    // create grid //
    /////////////////
    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++)
            m_grid.push_back({ (float)i, 0.0f, (float)j });

    Appbase::CreateVertexBuffer(m_vertexBufferGrid, m_grid);
    m_indexCountGrid = m_grid.size();

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
    vector<D3D11_INPUT_ELEMENT_DESC> inputLayout =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    CreateVertexShaderAndInputLayout(L"VS3D.hlsl", m_vertexShader, inputLayout, m_inputLayout);
    CreatePixelShader(L"PS3D.hlsl", m_pixelShader);

    // focus
    CreateVertexShaderAndInputLayout(L"VSFocus.hlsl", m_vertexShaderFocus, inputLayout, m_inputLayout);
    CreatePixelShader(L"PSFocus.hlsl", m_pixelShaderFocus);

    // normal
    CreateVertexShaderAndInputLayout(L"VSNormal.hlsl", m_vertexShaderNormal, inputLayout, m_inputLayout);
    CreatePixelShader(L"PSNormal.hlsl", m_pixelShaderNormal);

    // grid
    vector<D3D11_INPUT_ELEMENT_DESC> gridInputLayout =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    CreateVertexShaderAndInputLayout(L"VSGrid.hlsl", m_vertexShaderGrid, gridInputLayout, m_inputLayoutGrid);
    CreateGeometryShader(L"GSGrid.hlsl", m_geometryShaderGrid);
    CreatePixelShader(L"PSGrid.hlsl", m_pixelShaderGrid);

    return true;
}

void ExampleApp::Update()
{
    if (m_dimension == 2)
    {
        std::vector<glm::vec4> pixels(m_width * m_height, glm::vec4(m_initColor[0], m_initColor[1], m_initColor[2], m_initColor[3]));

        // update pixel constant buffer
        UpdateBuffer(m_pixelConstantBuffer2D, m_pixelConstantBufferData2D);

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
        if (m_appState == APP_STATE::HOME)
        {
            if (m_isScrolling)
            {
                // 마우스 스크롤링으로 물체 확대 및 축소
                
                m_viewDistance += m_mouseWheelDirection * m_viewDistance * 0.2f;
                m_viewDistance = clamp(m_viewDistance, 0.01f, 200.0f);

                m_isScrolling = false;
                m_isViewMoved = true;
            }

            if (m_isMButtonPressed)
            {
                // 마우스 스크롤 클릭 드래그로 특정 지점을 중심으로 시점 회전
                m_curMousePos = GetMousePos();

                if (!m_isDragging)
                {
                    // 드래그 시작 순간 진입하는 조건문
                    m_isDragging = true;
                    m_prevMousePos = m_curMousePos;

                    if (cos(m_viewPosAngle.y) > 0)
                        m_viewRotateDirection = -1;
                    else
                        m_viewRotateDirection = 1;

                }
                else if (m_isDragging)
                {
                    // 드래그 시작 이후 진입하는 조건문
                    Vector2 translation = m_curMousePos - m_prevMousePos;
                    translation /= 100.0f;
                    translation.x *= m_viewRotateDirection;

                    float translationX = m_curMousePos.x - m_prevMousePos.x;
                    float translationY = m_curMousePos.y - m_prevMousePos.y;

                    m_viewPosAngle.x += translation.x;
                    m_viewPosAngle.y += translation.y;

                    printf("viewPosAngle %f %f\n", m_viewPosAngle.x, m_viewPosAngle.y);

                    if (cos(m_viewPosAngle.y) > 0)
                        m_viewUp.y = 1.0f;
                    else
                        m_viewUp.y = -1.0f;

                    m_prevMousePos = m_curMousePos;
                }

                m_isViewMoved = true;
            }
        }
        else if (m_appState == APP_STATE::EDIT_SCALE)
        {
            m_curMousePos = GetMousePos();

            if (m_firstEntry)
            {
                m_firstEntry = false;

                m_prevScale = m_curScale;
                m_originToCursor = m_curMousePos.x * m_curMousePos.x + m_curMousePos.y * m_curMousePos.y;

                //m_prevMousePos = m_curMousePos;
            }
            else
            {
                float originToCursor = m_curMousePos.x * m_curMousePos.x + m_curMousePos.y * m_curMousePos.y;

                m_curScale = sqrt(originToCursor / m_originToCursor);
                m_curScale *= m_prevScale;

                m_modelScale = Vector3(m_curScale, m_curScale, m_curScale);

                //printf("scale %f\n", m_curScale);
            }

            m_isModelMoved = true;
        }
        else if (m_appState == APP_STATE::EDIT_ROTATE)
        {
            m_curMousePos = GetMousePos();

            if (m_firstEntry)
            {
                m_firstEntry = false;

                m_rotateAxis = m_viewLookAt - m_viewPos;
                m_prevRotateAngle = atan2(m_curMousePos.y, m_curMousePos.x);
                m_prevRotate = m_modelRotation;

                //m_prevMousePos = m_curMousePos;
            }
            else
            {
                m_curRotateAngle = atan2(m_curMousePos.y, m_curMousePos.x);
                m_curRotate = Matrix::CreateFromAxisAngle(m_rotateAxis, m_curRotateAngle - m_prevRotateAngle);

                m_modelRotation = m_prevRotate * m_curRotate;

                //printf("axis %f %f %f\n", m_rotateAxis.x, m_rotateAxis.y, m_rotateAxis.z);
                //printf("angle %f\n", m_curRotateAngle - m_prevRotateAngle);
            }

            m_isModelMoved = true;
        }
        else if (m_appState == APP_STATE::EDIT_TRANSLATE)
        {
            m_curMousePos = GetMousePos();

            if (m_firstEntry)
            {
                m_firstEntry = false;

                m_viewDir = m_viewLookAt - m_viewPos;

                Vector3 tempVector = Vector3(m_viewDir.x, 0.0f, m_viewDir.z);
                float f = tempVector.Length();

                m_viewNormalizedVertical.x = m_viewDir.x;
                m_viewNormalizedVertical.z = m_viewDir.z;
                if (abs(m_viewDir.y) < 1e-3f)
                    m_viewNormalizedVertical.y = 1.0f;
                else
                    m_viewNormalizedVertical.y = f * f / -m_viewDir.y;// 수직상방벡터의 방향에 영향을 받아 우측방벡터 방향이 뒤집히는 문제

                m_viewNormalizedVertical.Normalize();
                m_viewNormalizedVertical.Cross(m_viewDir, m_viewNormalizedRight);
                m_viewNormalizedRight.Normalize();

                m_prevTranslate = m_modelTranslation;

                m_prevMousePos = m_curMousePos;
            }
            else
            {
                float translateX = (m_curMousePos.x - m_prevMousePos.x) / 100.0f;
                float translateY = (m_curMousePos.y - m_prevMousePos.y) / 100.0f;
                
                m_curTranslate = Matrix::CreateTranslation(
                    Vector3(m_viewNormalizedRight.x * translateX + m_viewNormalizedVertical.x * translateY,
                        m_viewNormalizedVertical.y * translateY,
                        m_viewNormalizedRight.z * translateX + m_viewNormalizedVertical.z * translateY)
                );

                m_modelTranslation = m_curTranslate * m_prevTranslate;
                printf("viewdir y %f\n", m_viewDir.y);
                printf("vertical %f %f %f\n", m_viewNormalizedVertical.x, m_viewNormalizedVertical.y, m_viewNormalizedVertical.z);
                printf("right %f %f %f\n", m_viewNormalizedRight.x, m_viewNormalizedRight.y, m_viewNormalizedRight.z);
            }

            m_isModelMoved = true;
        }

        if (m_isModelMoved)
        {
            /////////////////////////
            // update model matrix //
            /////////////////////////
            m_vertexConstantBufferData.model = Matrix::CreateScale(m_modelScale) * m_modelRotation * m_modelTranslation;
            m_vertexConstantBufferData.model = m_vertexConstantBufferData.model.Transpose();

            m_vertexConstantBufferData.inverseTranspose = m_vertexConstantBufferData.model.Invert();
            m_vertexConstantBufferData.inverseTranspose = m_vertexConstantBufferData.inverseTranspose.Transpose();

            m_isModelMoved = false;
        }

        if (m_isViewMoved)
        {
            ////////////////////////
            // update view matrix //
            ////////////////////////
            m_viewPos.x = m_viewDistance * cos(m_viewPosAngle.y) * sin(m_viewPosAngle.x);
            m_viewPos.y = -m_viewDistance * sin(m_viewPosAngle.y);
            m_viewPos.z = -m_viewDistance * cos(m_viewPosAngle.y) * cos(m_viewPosAngle.x);

            m_vertexConstantBufferData.view = XMMatrixLookAtLH(m_viewPos, m_viewLookAt, m_viewUp);
            //m_vertexConstantBufferData.view = XMMatrixLookToLH(m_viewPos, m_viewDir, m_viewUp);
            m_vertexConstantBufferData.view = m_vertexConstantBufferData.view.Transpose();

            m_pixelConstantBufferData.eyePosition = m_viewPos;

            m_isViewMoved = false;
        }

        //////////////////////////////
        // update projection matrix //
        //////////////////////////////
        m_vertexConstantBufferData.projection = XMMatrixPerspectiveFovLH(
            XMConvertToRadians(m_fieldOfViewAngle), GetAspectRatio(), m_nearZ, m_farZ);
        m_vertexConstantBufferData.projection = m_vertexConstantBufferData.projection.Transpose();

        UpdateBuffer(m_meshes[0]->vertexConstantBuffer, m_vertexConstantBufferData);
        UpdateBuffer(m_meshes[0]->pixelConstantBuffer, m_pixelConstantBufferData);

        //////////////////
        // update focus //
        //////////////////
        m_vertexConstantBufferDataFocus.modelFocus = 
            Matrix::CreateScale(m_modelScale) *
            Matrix::CreateScale(1.1f) *
            m_modelRotation *
            m_modelTranslation;
        m_vertexConstantBufferDataFocus.modelFocus = m_vertexConstantBufferDataFocus.modelFocus.Transpose();

        UpdateBuffer(m_vertexConstantBufferFocus, m_vertexConstantBufferDataFocus);

        ///////////////////
        // update normal //
        ///////////////////
        if (m_drawNormal)
        {
            UpdateBuffer(m_normals->vertexConstantBuffer, m_vertexConstantBufferDataNormal);
        }

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
    //m_context->ClearRenderTargetView(m_baseRTV.Get(), m_initColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
    m_context->RSSetState(m_rasterizerState.Get());

    if (m_dimension == 2)
    {
        //
        m_context->ClearRenderTargetView(m_baseRTV.Get(), m_initColor);
        //

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
            m_context->PSSetShaderResources(0, (UINT)m_imageSRVs.size(), m_imageSRVs[0].GetAddressOf());
        }
        else
        {
            std::vector<ID3D11ShaderResourceView*> nullSRV((UINT)m_imageSRVs.size(), nullptr);
            m_context->PSSetShaderResources(0, (UINT)nullSRV.size(), nullSRV.data());
            m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());
        }

        m_context->PSSetConstantBuffers(0, 1, m_pixelConstantBuffer2D.GetAddressOf());
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_context->DrawIndexed((UINT)m_indexCount2D, 0, 0);
    }
    else if (m_dimension == 3)
    {
        //
        m_context->ClearRenderTargetView(m_baseRTV.Get(), m_backgroundColor);
        //
        
        if (m_drawWireframe)
        {
            m_context->RSSetState(m_rasterizerStateWireframe.Get());
        }
         
        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

        std::vector<ID3D11ShaderResourceView*> nullSRV(m_imageSRVs.size(), nullptr);
        m_context->PSSetShaderResources(0, (UINT)nullSRV.size(), nullSRV.data());
        m_context->PSSetShaderResources(0, 1, m_canvasSRV.GetAddressOf());

        // 그리드 그리기 -> 모델 포커스 그리기 -> 모델 그리기
        // draw grid
        {
            m_context->IASetInputLayout(m_inputLayoutGrid.Get());
            m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
            //m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

            UINT stride = sizeof(Vector3);
            UINT offset = 0;

            m_context->VSSetShader(m_vertexShaderGrid.Get(), 0, 0);
            m_context->IASetVertexBuffers(0, 1, m_vertexBufferGrid.GetAddressOf(), &stride, &offset);

            m_context->GSSetShader(m_geometryShaderGrid.Get(), 0, 0);
            vector<ComPtr<ID3D11Buffer>> geometryConstantBuffers = {
                m_vertexConstantBuffer,
                m_pixelConstantBuffer
            };
            m_context->GSSetConstantBuffers(0, geometryConstantBuffers.size(), geometryConstantBuffers.data()->GetAddressOf());

            m_context->PSSetShader(m_pixelShaderGrid.Get(), 0, 0);

            m_context->Draw((UINT)m_indexCountGrid, 0);

            m_context->GSSetShader(nullptr, 0, 0);
        }

        // draw focus
        {
            //m_context->IASetInputLayout(m_inputLayoutGrid.Get());
            //m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            //
            //UINT stride = sizeof(Vertex3D);
            //UINT offset = 0;
            //
            //m_context->VSSetShader(m_vertexShaderFocus.Get(), 0, 0);
            //m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
            //m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
            //vector<ComPtr<ID3D11Buffer>> vertexConstantBuffers = {
            //    m_vertexConstantBuffer,
            //    m_vertexConstantBufferFocus
            //};
            //m_context->VSSetConstantBuffers(0, vertexConstantBuffers.size(), vertexConstantBuffers.data()->GetAddressOf());
            //
            //m_context->PSSetShader(m_pixelShaderFocus.Get(), 0, 0);
            //m_context->PSSetConstantBuffers(0, 1, m_pixelConstantBuffer.GetAddressOf());
            //
            //m_context->DrawIndexed((UINT)m_indexCount, 0, 0);
        }

        // draw model
        for (const auto& mesh : m_meshes)
        {
            m_context->IASetInputLayout(m_inputLayout.Get());
            m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            UINT stride = sizeof(Vertex3D);
            UINT offset = 0;

            m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
            m_context->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
            m_context->IASetIndexBuffer(mesh->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
            m_context->VSSetConstantBuffers(0, 1, mesh->vertexConstantBuffer.GetAddressOf());

            m_context->PSSetShader(m_pixelShader.Get(), 0, 0);
            m_context->PSSetConstantBuffers(0, 1, mesh->pixelConstantBuffer.GetAddressOf());

            m_context->DrawIndexed((UINT)mesh->indexCount, 0, 0);
        }

        // draw normal
        if (m_drawNormal)
        {
            m_context->IASetInputLayout(m_inputLayout.Get());
            m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

            UINT stride = sizeof(Vertex3D);
            UINT offset = 0;

            m_context->VSSetShader(m_vertexShaderNormal.Get(), 0, 0);
            m_context->IASetVertexBuffers(0, 1, m_normals->vertexBuffer.GetAddressOf(), &stride, &offset);
            m_context->IASetIndexBuffer(m_normals->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
            vector<ComPtr<ID3D11Buffer>> normalVertexConstantBuffers = {
                m_meshes[0]->vertexConstantBuffer,
                m_normals->vertexConstantBuffer
            };
            m_context->VSSetConstantBuffers(0, normalVertexConstantBuffers.size(), normalVertexConstantBuffers.data()->GetAddressOf());

            m_context->PSSetShader(m_pixelShaderNormal.Get(), 0, 0);
                
            m_context->DrawIndexed((UINT)m_normals->indexCount, 0, 0);
        }   
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
        ImGui::SliderFloat("Threshold", &m_pixelConstantBufferData2D.threshold, 0.0f, 1.0f);
    }
    else if (m_dimension == 3)
    {
        ImGui::Checkbox("Draw Wireframe", &m_drawWireframe);
        ImGui::Checkbox("Draw Normal", &m_drawNormal);
        ImGui::SliderFloat("Normal Scale", &m_vertexConstantBufferDataNormal.scale, 0.0, 1.0);

        if (m_appState == APP_STATE::HOME)
        {
            ImGui::Text("Box");
            //ImGui::SliderFloat3("Mesh Position", &m_modelTranslation.x, -2.0f, 2.0f);
            ImGui::SliderFloat("View Distance", &m_viewDistance, -10.f, 10.f);
            ImGui::SliderFloat("Field Of View", &m_fieldOfViewAngle, 50.0f, 150.0f);
            //ImGui::SliderFloat("Near Z", &m_nearZ, 0.01f, 10.0f);
            //ImGui::SliderFloat("Far Z", &m_farZ, m_nearZ + 0.01f, 1000.0f);
            //ImGui::SliderFloat("Aspect Ratio", &m_aspectRatio, 0.1f, 3.0f);
        }
        else if (m_appState == APP_STATE::EDIT_SCALE)
        {
            ImGui::Text("Scale");
            ImGui::SliderFloat("Mesh Scale", &m_curScale, 0.0f, 10.0f);
        }
        else if (m_appState == APP_STATE::EDIT_ROTATE)
        {
            ImGui::Text("Rotate");
            //ImGui::SliderFloat3("Mesh Rotate", &m_modelRotation.x, -3.0f, 3.0f);
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
        {
            m_appState = APP_STATE::HOME;
            m_firstEntry = true;
        }
    }
    else if (m_appState == APP_STATE::EDIT_ROTATE)
    {
        if (keyPressed == 27)
        {
            m_appState = APP_STATE::HOME;
            m_firstEntry = true;
        }
    }
    else if (m_appState == APP_STATE::EDIT_TRANSLATE)
    {
        if (keyPressed == 27)
        {
            m_appState = APP_STATE::HOME;
            m_firstEntry = true;
        }
    }

}
