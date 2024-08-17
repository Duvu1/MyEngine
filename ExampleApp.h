#pragma once

#include "Appbase.h"
#include "Circle.h"
#include "Image.h"
#include "Model.h"
#include "Raytracer.h"

enum class APP_STATE
{
    HOME,
    EDIT_SCALE,
    EDIT_ROTATE,
    EDIT_TRANSLATE,

    END
};

struct VSConstantBufferData
{
    Matrix model = Matrix();
    Matrix inverseTranspose = Matrix();
    Matrix view = Matrix();
    Matrix projection = Matrix();
};

struct NormalConstantBufferData
{
    float scale = 0.5;
    float dummy[3];
};

struct PSConstantBufferData
{
    Vector3 eyePosition;
    float dummy;
};

struct FocusConstantBufferData
{
    Matrix scale = Matrix();
};

struct PSConstantBufferData2D
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

private:
    bool InitShaders();

    void Update();
    void Render();
    void UpdateGUI();

    void KeyControl(int keyPressed);

private:
    int m_dimension = 3;
    bool m_textureOn = false;
    bool m_drawNormal = true;
    bool m_drawWireframe = false;
    APP_STATE m_appState = APP_STATE::HOME;

    std::unique_ptr<Raytracer> m_raytracer;

    // buffer
    ComPtr<ID3D11Buffer> m_vertexBuffer2D;
    ComPtr<ID3D11Buffer> m_indexBuffer2D;
    size_t m_indexCount2D;

    ComPtr<ID3D11Buffer> m_vertexBuffer3D;
    ComPtr<ID3D11Buffer> m_indexBuffer3D;
    size_t m_indexCount3D;

    ComPtr<ID3D11Buffer> m_vertexBufferNormal3D;
    ComPtr<ID3D11Buffer> m_indexBufferNormal3D;
    size_t m_indexCountNormal3D;

    ComPtr<ID3D11Buffer> m_vertexBufferGrid;
    size_t m_indexCountGrid;

    ComPtr<ID3D11Buffer> m_vertexConstantBuffer;
    ComPtr<ID3D11Buffer> m_pixelConstantBuffer;
    ComPtr<ID3D11Buffer> m_normalVertexConstantBuffer;
    ComPtr<ID3D11Buffer> m_geometryConstantBuffer;
    ComPtr<ID3D11Buffer> m_focusConstantBuffer;

    ComPtr<ID3D11Buffer> m_pixelConstantBuffer2D;

    // MVP
    Vector3 m_modelScale = Vector3(1.0f);
    Vector3 m_modelRotation = Vector3(0.0f);
    Vector3 m_modelTranslation = Vector3(0.0f);

    bool m_isViewMoved = true;

    glm::vec2 m_viewPosAngle = { 0.0f, 0.0f };
    int m_viewRotateDirection = -1; // 보통 회전:-1, 화면 상하 반전 시 회전:1
    float m_viewDistance = 5.0f;
    
    Vector3 m_viewPos = { 0.0f, 0.0f, -m_viewDistance };
    Vector3 m_viewDir = { 0.0f, 0.0f, 1.0f };
    Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };
    Vector3 m_viewLookAt = { 0.0f, 0.0f, 0.0f };

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
    NormalConstantBufferData m_normalVertexConstantBufferData;
    FocusConstantBufferData m_focusConstantBufferData;

    PSConstantBufferData2D m_pixelConstantBufferData2D;

    // objects
    std::vector<Vector3> m_grid =
    {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }
    };
    std::unique_ptr<Circle> m_circle;
    std::unique_ptr<Square> m_square;
    std::vector<std::shared_ptr<Model>> m_models;

    // mouse
    Vector2 m_prevMousePos = { 0, 0 };
    Vector2 m_curMousePos = { 0, 0 };
};

