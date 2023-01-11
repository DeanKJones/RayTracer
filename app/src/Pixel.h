#ifndef RAYTRACING_PIXEL_H
#define RAYTRACING_PIXEL_H

#include "glm/glm.hpp"
#include "../../core/src/Image.h"

class Pixel
{
public:
    Pixel() = default;
    ~Pixel() = default;

    Pixel(int x, int y);
    Pixel(glm::vec2 pPixelPosition);

public:
    glm::vec2 position;
    glm::vec4 RGBA = {0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec3 RGB  = {0.0f, 0.0f, 0.0f};
};


#endif //RAYTRACING_PIXEL_H
