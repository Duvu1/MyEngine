#pragma once

#include "framework.h"

class Image
{
public:
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<glm::vec4> pixels;

    void ReadFromFile(const char* filename);
};