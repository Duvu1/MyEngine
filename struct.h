#pragma once

#include "framework.h"

struct Vertex2D
{
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec2 uv;
};

struct Vertex3D
{
    Vector3 position;
    Vector3 normal;
    Vector2 texcoord;
};