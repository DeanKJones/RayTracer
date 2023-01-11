
#include "Pixel.h"

Pixel::Pixel(int x, int y)
{
    position = glm::vec2(x, y);
}

Pixel::Pixel(glm::vec2 pPixelPosition)
{
    position = pPixelPosition;
}