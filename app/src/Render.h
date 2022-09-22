#pragma once

#include "../../core/src/Image.h"
#include "../../core/src/Random.h"

#include "Camera.h"
#include "Ray.h"
#include "obj/Object.h"

#include "glm/glm.hpp"
#include <memory>

class Renderer
{
public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void Render(const Camera& camera, const std::vector<std::unique_ptr<Object>> &objects);

    std::shared_ptr<Core::Image> getFinalImage() const { return m_FinalImage; }

private:
    bool TraceRay(const Ray& ray, const std::vector<std::unique_ptr<Object>> &objects, float &tNear, const Object *&hitObject);
    glm::vec4 RenderColor(const Ray& ray, const std::vector<std::unique_ptr<Object>> &objects);
    uint32_t ConvertRGBA(glm::vec4 color);

private:
    // Image
    std::shared_ptr<Core::Image> m_FinalImage;
    uint32_t* m_imageData = nullptr;

public:     // Getter Functions 
    glm::vec3 GetLightDirection() { return lightDirection; }
    static glm::vec3 lightDirection;
};