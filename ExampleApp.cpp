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

    m_camera = Camera();
    m_camera.SetAspectRatio(m_width, m_height);
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
                m_camera.SetCameraDistance(m_mouseWheelDirection);

                m_isScrolling = false;
            }

            if (m_isMButtonPressed)
            {
                // 마우스 스크롤 클릭 드래그로 특정 지점을 중심으로 시점 회전
                m_curMousePos = GetMousePos();

                if (!m_isDragging)
                {
                    // 드래그 시작 순간 진입하는 조건문
                    m_isDragging = true;
                    m_screenVerticalDir = m_camera.GetViewVertical();

                    m_prevMousePos = m_curMousePos;
                }
                else if (m_isDragging)
                {
                    // 드래그 시작 이후 진입하는 조건문
                    Vector2 dMousePos = m_curMousePos - m_prevMousePos;
                    dMousePos /= 100.0f;

                    if (m_screenVerticalDir.y < 0)
                        dMousePos.x *= -1;

                    m_camera.SetCameraRotation(dMousePos);

                    m_prevMousePos = m_curMousePos;
                }
            }

            m_isViewMoved = true;
        }
        else if (m_appState == APP_STATE::EDIT_VIEW)
        {
            if (m_isScrolling)
            {
                // 마우스 스크롤링으로 물체 확대 및 축소
                m_camera.SetCameraDistance(m_mouseWheelDirection);

                m_isScrolling = false;
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
                }
                else if (m_isDragging)
                {
                    // 드래그 시작 이후 진입하는 조건문
                    Vector2 dMousePos = m_curMousePos - m_prevMousePos;
                    dMousePos /= 100.0f;

                    m_camera.SetCameraPosition(dMousePos);

                    m_prevMousePos = m_curMousePos;
                }
            }

            m_isViewMoved = true;
        }
        else if (m_appState == APP_STATE::EDIT_SCALE)
        {
            m_curMousePos = GetMousePos();

            if (m_firstEntry)
            {
                m_firstEntry = false;

                m_prevScale = m_finalScale;
                m_originToCursor = m_curMousePos.x * m_curMousePos.x + m_curMousePos.y * m_curMousePos.y;

                //m_prevMousePos = m_curMousePos;
            }
            else
            {
                float originToCursor = m_curMousePos.x * m_curMousePos.x + m_curMousePos.y * m_curMousePos.y;
                float scale = sqrt(originToCursor / m_originToCursor);

                if (m_editState == EDIT_STATE::DEFAULT)
                    m_curScale = Matrix::CreateScale(Vector3(scale));
                else if (m_editState == EDIT_STATE::EDIT_X)
                    m_curScale = Matrix::CreateScale(Vector3(scale, 1.0f, 1.0f));
                else if (m_editState == EDIT_STATE::EDIT_Y)
                    m_curScale = Matrix::CreateScale(Vector3(1.0f, scale, 1.0f));
                else if (m_editState == EDIT_STATE::EDIT_Z)
                    m_curScale = Matrix::CreateScale(Vector3(1.0f, 1.0f, scale));

                m_finalScale = m_curScale * m_prevScale;

                m_modelScale = m_finalScale.ToEuler();

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

                m_rotateAxis = m_camera.GetViewDirection();
                m_rotateAxis.Normalize();
                m_prevRotateAngle = atan2(m_curMousePos.y, m_curMousePos.x);
                m_prevRotate = m_finalRotate;

                //m_prevMousePos = m_curMousePos;
            }
            else
            {
                m_curRotateAngle = atan2(m_curMousePos.y, m_curMousePos.x);
                //m_curRotate = Matrix::CreateFromAxisAngle(m_rotateAxis, m_curRotateAngle - m_prevRotateAngle);

                Quaternion q;
                if (m_editState == EDIT_STATE::DEFAULT)
                    q = Quaternion::CreateFromAxisAngle(m_rotateAxis, m_curRotateAngle - m_prevRotateAngle);
                else if (m_editState == EDIT_STATE::EDIT_X)
                    q = Quaternion::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), m_curRotateAngle - m_prevRotateAngle);
                else if (m_editState == EDIT_STATE::EDIT_Y)
                    q = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), m_curRotateAngle - m_prevRotateAngle);
                else if (m_editState == EDIT_STATE::EDIT_Z)
                    q = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), m_curRotateAngle - m_prevRotateAngle);

                m_curRotate = Matrix::CreateFromQuaternion(q);
                m_finalRotate = m_prevRotate * m_curRotate;

                m_modelRotation = m_finalRotate.ToEuler();
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

                m_screenViewDir = m_camera.GetViewDirection();
                m_screenVerticalDir = m_camera.GetViewVertical();
                m_screenRightDir = m_camera.GetViewRight();

                m_prevTranslate = m_modelTranslation;

                m_prevMousePos = m_curMousePos;
            }
            else
            {
                Vector2 dMousePos = m_curMousePos - m_prevMousePos;
                dMousePos /= 100.0f;

                if (m_editState == EDIT_STATE::DEFAULT)
                    m_curTranslate = Matrix::CreateTranslation(
                        Vector3(m_screenRightDir.x * dMousePos.x + m_screenVerticalDir.x * dMousePos.y,
                            m_screenVerticalDir.y * dMousePos.y,
                            m_screenRightDir.z * dMousePos.x + m_screenVerticalDir.z * dMousePos.y)
                    );
                else if (m_editState == EDIT_STATE::EDIT_X)
                    m_curTranslate = Matrix::CreateTranslation(Vector3(m_screenRightDir.x * dMousePos.x + m_screenVerticalDir.x * dMousePos.y, 0.0f, 0.0f));
                else if (m_editState == EDIT_STATE::EDIT_Y)
                    m_curTranslate = Matrix::CreateTranslation(Vector3(0.0f, m_screenVerticalDir.y * dMousePos.y, 0.0f));
                else if (m_editState == EDIT_STATE::EDIT_Z)
                    m_curTranslate = Matrix::CreateTranslation(Vector3(0.0f, 0.0f, m_screenRightDir.z * dMousePos.x + m_screenVerticalDir.z * dMousePos.y));

                m_modelTranslation = m_curTranslate * m_prevTranslate;

                //printf("viewdir y %f\n", m_screenViewDir.y);
                //printf("vertical %f %f %f\n", m_screenVerticalDir.x, m_screenVerticalDir.y, m_screenVerticalDir.z);
                //printf("right %f %f %f\n", m_screenRightDir.x, m_screenRightDir.y, m_screenRightDir.z);
            }

            m_isModelMoved = true;
        }

        if (m_isModelMoved)
        {
            /////////////////////////
            // update model matrix //
            /////////////////////////
            m_vertexConstantBufferData.model = m_finalScale * m_finalRotate * m_modelTranslation;
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
            m_vertexConstantBufferData.view = m_camera.GetViewMatrix();
            m_vertexConstantBufferData.view = m_vertexConstantBufferData.view.Transpose();
            m_pixelConstantBufferData.eyePosition = m_camera.GetCameraPosition();

            m_isViewMoved = false;
        }

        //////////////////////////////
        // update projection matrix //
        //////////////////////////////
        m_vertexConstantBufferData.projection = m_camera.GetProjectionMatrix();
        m_vertexConstantBufferData.projection = m_vertexConstantBufferData.projection.Transpose();

        UpdateBuffer(m_meshes[0]->vertexConstantBuffer, m_vertexConstantBufferData);
        UpdateBuffer(m_meshes[0]->pixelConstantBuffer, m_pixelConstantBufferData);

        //////////////////
        // update focus //
        //////////////////
        m_vertexConstantBufferDataFocus.modelFocus = 
            Matrix::CreateScale(m_modelScale) *
            Matrix::CreateScale(1.1f) *
            m_finalRotate *
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
        }
        else if (m_appState == APP_STATE::EDIT_SCALE)
        {
            ImGui::Text("Scale");
            ImGui::SliderFloat3("Mesh Scale", &m_modelScale.x, 0.0f, 10.0f);
        }
        else if (m_appState == APP_STATE::EDIT_ROTATE)
        {
            ImGui::Text("Rotate");
            ImGui::SliderFloat3("Mesh Rotate", &m_modelRotation.x, -3.15f, 3.15f);
        }
        else if (m_appState == APP_STATE::EDIT_TRANSLATE)
        {
            ImGui::Text("Translate");
            //ImGui::SliderFloat3("Mesh Translate", &m_modelTranslation.x, -3.15f, 3.15f);
        }
        else if (m_appState == APP_STATE::EDIT_VIEW)
        {
            ImGui::Text("View");
            //
        }
    }   
}

void ExampleApp::KeyControl(int keyPressed)
{
    if (m_appState == APP_STATE::HOME)
    {
        if (keyPressed == 27)
            DestroyWindow(m_hWnd);
        else if (keyPressed == 'V')
            m_appState = APP_STATE::EDIT_VIEW;
        else if (keyPressed == 'S')
            m_appState = APP_STATE::EDIT_SCALE;
        else if (keyPressed == 'R')
            m_appState = APP_STATE::EDIT_ROTATE;
        else if (keyPressed == 'T')
            m_appState = APP_STATE::EDIT_TRANSLATE;
    }
    else if (m_appState == APP_STATE::EDIT_VIEW)
    {
        if (keyPressed == 27)
        {
            m_appState = APP_STATE::HOME;
            m_editState = EDIT_STATE::DEFAULT;
        }
    }
    else if (m_appState == APP_STATE::EDIT_SCALE)
    {
        if (keyPressed == 27)
        {
            m_appState = APP_STATE::HOME;
            m_editState = EDIT_STATE::DEFAULT;
            m_firstEntry = true;
        }
        else if (keyPressed == 'X')
        {
            m_editState = EDIT_STATE::EDIT_X;
        }
        else if (keyPressed == 'Y')
        {
            m_editState = EDIT_STATE::EDIT_Y;
        }
        else if (keyPressed == 'Z')
        {
            m_editState = EDIT_STATE::EDIT_Z;
        }
    }
    else if (m_appState == APP_STATE::EDIT_ROTATE)
    {
        if (keyPressed == 27)
        {
            m_appState = APP_STATE::HOME;
            m_editState = EDIT_STATE::DEFAULT;
            m_firstEntry = true;
        }
        else if (keyPressed == 'X')
        {
            m_editState = EDIT_STATE::EDIT_X;
        }
        else if (keyPressed == 'Y')
        {
            m_editState = EDIT_STATE::EDIT_Y;
        }
        else if (keyPressed == 'Z')
        {
            m_editState = EDIT_STATE::EDIT_Z;
        }
    }
    else if (m_appState == APP_STATE::EDIT_TRANSLATE)
    {
        if (keyPressed == 27)
        {
            m_appState = APP_STATE::HOME;
            m_editState = EDIT_STATE::DEFAULT;
            m_firstEntry = true;
        }
        else if (keyPressed == 'X')
        {
            m_editState = EDIT_STATE::EDIT_X;
        }
        else if (keyPressed == 'Y')
        {
            m_editState = EDIT_STATE::EDIT_Y;
        }
        else if (keyPressed == 'Z')
        {
            m_editState = EDIT_STATE::EDIT_Z;
        }
    }
}
