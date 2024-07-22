#include "Appbase.h"

#include <omp.h> // open mp, multi-threading

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//#define STB_IMAGE_WRITE_IMPLAMENTATION
//#include <stb_image_write.h>

void Image::ReadFromFile(const char* filename)
{
    unsigned char* img = stbi_load(filename, &width, &height, &channels, 0);

    if (width) {
        std::cout << width << " " << height << " " << channels << std::endl;
    }
    else {
        std::cout << "Error: reading " << filename << " failed." << std::endl;
    }

    pixels.resize(width * height);

    for (int i = 0; i < width * height; i++)
    {
        pixels[i][0] = img[i * channels] / 255.0f;
        pixels[i][1] = img[i * channels + 1] / 255.0f;
        pixels[i][2] = img[i * channels + 2] / 255.0f;
        pixels[i][3] = 1.0f;
    }

    delete[] img;
}