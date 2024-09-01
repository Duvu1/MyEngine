#pragma once

#include "struct.h"
#include "Appbase.h"
#include "Circle.h"
#include "Image.h"
#include "MakeMesh.h"
#include "Mesh.h"
#include "Raytracer.h"

enum class APP_STATE
{
    HOME,
    EDIT_VIEW,
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

struct FocusConstantBufferData
{
    Matrix modelFocus = Matrix();
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
    bool m_drawNormal = false;
    bool m_drawWireframe = false;
    APP_STATE m_appState = APP_STATE::HOME;
    bool m_firstEntry = true;

    std::unique_ptr<Raytracer> m_raytracer;

    // buffer
    ComPtr<ID3D11Buffer> m_vertexBuffer2D;
    ComPtr<ID3D11Buffer> m_indexBuffer2D;
    size_t m_indexCount2D;

    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    size_t m_indexCount;

    ComPtr<ID3D11Buffer> m_vertexBufferNormal;
    ComPtr<ID3D11Buffer> m_indexBufferNormal;
    size_t m_indexCountNormal;

    ComPtr<ID3D11Buffer> m_vertexBufferGrid;
    size_t m_indexCountGrid;

    ComPtr<ID3D11Buffer> m_vertexConstantBuffer;
    ComPtr<ID3D11Buffer> m_pixelConstantBuffer;
    ComPtr<ID3D11Buffer> m_vertexConstantBufferFocus;
    ComPtr<ID3D11Buffer> m_vertexConstantBufferNormal;
    ComPtr<ID3D11Buffer> m_geometryConstantBufferGrid;

    ComPtr<ID3D11Buffer> m_pixelConstantBuffer2D;

    // MVP
    Vector3 m_modelScale = Vector3(1.0f);
    Vector3 m_modelRotation = Vector3(0.0f);
    Matrix m_modelTranslation = Matrix();

    // // scale
    float m_originToCursor = 0.0f;
    float m_prevScale = 1.0f;
    float m_curScale = 1.0f;

    // // rotate
    Vector3 m_rotateAxis = Vector3(0.0f);
    float m_prevRotateAngle = 0.0f;
    float m_curRotateAngle = 0.0f;
    Matrix m_prevRotate = Matrix();
    Matrix m_curRotate = Matrix();
    Matrix m_finalRotate = Matrix();

    // // translation
    Matrix m_prevTranslate = Matrix();
    Matrix m_curTranslate = Matrix();

    // // dirty flag
    bool m_isModelMoved = true;
    bool m_isViewMoved = true;

    // screen
    Vector3 m_screenViewDir = Vector3(0.0f, 0.0f, 1.0f);
    Vector3 m_screenVerticalDir = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 m_screenRightDir = Vector3(1.0f, 0.0, 0.0f);

    // texture
    ComPtr<ID3D11SamplerState> m_samplerState;
    ComPtr<ID3D11Texture2D> m_canvasTexture;

    std::vector<Image> m_images;
    std::vector<ComPtr<ID3D11Texture2D>> m_imageTextures;

    // constant buffer data
    VSConstantBufferData m_vertexConstantBufferData;
    PSConstantBufferData m_pixelConstantBufferData;
    FocusConstantBufferData m_vertexConstantBufferDataFocus;
    NormalConstantBufferData m_vertexConstantBufferDataNormal;

    PSConstantBufferData2D m_pixelConstantBufferData2D;

    // objects
    std::vector<Vector3> m_grid;
    std::unique_ptr<Circle> m_circle;
    std::unique_ptr<Square> m_square;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Mesh> m_normals;

    // mouse
    Vector2 m_prevMousePos = { 0, 0 };
    Vector2 m_curMousePos = { 0, 0 };
};

