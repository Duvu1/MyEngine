#pragma once

#include "Appbase.h"
#include "Circle.h"
#include "Image.h"
#include "Raytracer.h"

struct VSConstantBufferData
{
    Matrix view;
};

struct PSConstantBufferData
{
    float threshold = 0.0f;
    float dummy[3];
};

class ExampleApp : public Appbase
{
public:
    ExampleApp();
    ~ExampleApp();

    bool Initialize();
    bool InitShaders();

    void Update();
    void Render();
    void UpdateGUI();

public:
    int m_dimension = 2;
    std::unique_ptr<Raytracer> m_raytracer;

    ComPtr<ID3D11Buffer> m_vertexBuffer2D;
    ComPtr<ID3D11Buffer> m_vertexBuffer3D;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    UINT indexCount;
    ComPtr<ID3D11Buffer> m_vertexConstantBuffer;
    ComPtr<ID3D11Buffer> m_pixelConstantBuffer;

    Vector3 m_viewPos = { 0.0f, 0.0f, -1.0f };
    Vector3 m_viewDir = { 0.0f, 0.0f, 1.0f };
    Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };

    ComPtr<ID3D11Texture2D> m_canvasTexture;

    std::vector<Image> m_images;
    std::vector<ComPtr<ID3D11Texture2D>> m_imageTextures;

    VSConstantBufferData m_vertexConstantBufferData;
    PSConstantBufferData m_pixelConstantBufferData;

    std::unique_ptr<Circle> m_circle;

    bool m_textureOn = false;

    glm::vec2 m_prevMousePos = { 0, 0 };
    glm::vec2 m_curMousePos;
};

