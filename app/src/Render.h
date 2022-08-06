#pragma once

#include "../../core/src/Image.h"
#include "../../core/src/Random.h"

#include "glm/glm.hpp"
#include <memory>

class Renderer
{
public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void Render();

    std::shared_ptr<Core::Image> getFinalImage() const { return m_FinalImage; }

private:
    glm::vec4 perPixel(glm::vec2 coord);

private:
    std::shared_ptr<Core::Image> m_FinalImage;
    uint32_t* m_imageData = nullptr;
};