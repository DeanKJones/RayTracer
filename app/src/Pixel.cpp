
#include "Pixel.h"

Pixel::Pixel(uint32_t x, uint32_t y)
{
    coord = glm::vec2(x, y);
}

Pixel::Pixel(glm::vec2 pPixelPosition)
{
    coord = pPixelPosition;
}