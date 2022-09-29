#pragma once

#include "../../core/src/Image.h"
#include "../../core/src/Random.h"

#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

#include "glm/glm.hpp"
#include <memory>
#include <cstdlib>

class Renderer
{
public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void Render(const Camera& camera, const Scene& scene);

    std::shared_ptr<Core::Image> getFinalImage() const { return m_FinalImage; }
    
    glm::vec3 GetLightDirection() const { return lightDirection; }
    int GetSamplesPerPixel() const { return samplesPerPixel; }
    int GetBounceDepth() const { return bounceDepth; }
    bool GetGiTag() const { return doGI; }
    bool GetRenderMode() const { return renderEachFrame; }

private:
    // Passing hitObject by a reference to a pointer of type const Object
    // This allows the object to get modified without creating a new object
    bool TraceRay(const Ray& ray, const Scene& scene, float &tNear, const Object *&hitObject);
    glm::vec4 RenderColor(Ray& ray, const Scene& scene, int depth);
    uint32_t ConvertRGBA(glm::vec4 color, int& samples);

private:
    // Image
    std::shared_ptr<Core::Image> m_FinalImage;
    uint32_t* m_imageData = nullptr;

public:
    static glm::vec3 lightDirection;
    static int samplesPerPixel;
    static int bounceDepth;
    static bool doGI;
    static bool renderEachFrame;
};