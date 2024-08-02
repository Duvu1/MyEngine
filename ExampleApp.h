#pragma once

#include "Appbase.h"
#include "Circle.h"
#include "Image.h"
#include "Model.h"
#include "Raytracer.h"

struct VSConstantBufferData
{
    Matrix model = Matrix();
    Matrix view = Matrix();
    Matrix projection = Matrix();
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
    int m_dimension = 3;
    bool m_textureOn = false;

    std::unique_ptr<Raytracer> m_raytracer;

    // buffer
    ComPtr<ID3D11Buffer> m_vertexBuffer2D;
    ComPtr<ID3D11Buffer> m_vertexBuffer3D;
    ComPtr<ID3D11Buffer> m_indexBuffer2D;
    ComPtr<ID3D11Buffer> m_indexBuffer3D;
    size_t m_indexCount2D;
    size_t m_indexCount3D;
    ComPtr<ID3D11Buffer> m_vertexConstantBuffer;
    ComPtr<ID3D11Buffer> m_pixelConstantBuffer;

    // MVP
    Vector3 m_modelTranslation = Vector3(0.0f);

    Vector3 m_viewPos = { 0.0f, 0.0f, -5.0f };
    Vector3 m_viewDir = { 0.0f, 0.0f, 1.0f };
    Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };

    float m_fieldOfViewAngle = 90.0f;
    float m_nearZ = 0.01f;
    float m_farZ = 100.0f;
    float m_aspectRatio = GetAspectRatio();

    // texture
    ComPtr<ID3D11SamplerState> m_samplerState;
    ComPtr<ID3D11Texture2D> m_canvasTexture;

    std::vector<Image> m_images;
    std::vector<ComPtr<ID3D11Texture2D>> m_imageTextures;

    // constant buffer data
    VSConstantBufferData m_vertexConstantBufferData;
    PSConstantBufferData m_pixelConstantBufferData;

    // objects
    std::unique_ptr<Circle> m_circle;
    std::unique_ptr<Square> m_square;
    std::unique_ptr<Model> m_model;

    // mouse
    glm::vec2 m_prevMousePos = { 0, 0 };
    glm::vec2 m_curMousePos = { 0, 0 };
};

