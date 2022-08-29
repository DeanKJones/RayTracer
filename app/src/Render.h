#pragma once

#include "../../core/src/Image.h"
#include "../../core/src/Random.h"

#include "Camera.h"
#include "Ray.h"

#include "glm/glm.hpp"
#include <memory>

class Renderer
{
public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void Render(const Camera& camera);

    std::shared_ptr<Core::Image> getFinalImage() const { return m_FinalImage; }

private:
    glm::vec4 TraceRay(const Ray& ray);
    uint32_t ConvertRGBA(glm::vec4 color);

private:
    void createSphere(glm::vec3 position, float radius);
    glm::vec3 s_pos;
    float s_radius;

private:
    std::shared_ptr<Core::Image> m_FinalImage;
    uint32_t* m_imageData = nullptr;
};

namespace Utils {
    // IMGUI GLOBALS
    static glm::vec3 SphereColor{1.0f, 0.0f, 0.0f};
    static glm::vec3 lightDirection{0.0f, 0.0f, 0.0f};
   
}