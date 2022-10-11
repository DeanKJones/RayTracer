#pragma once

#include "../../core/src/Image.h"

#include "Camera.h"
#include "Scene.h"
#include "Material.h"

#include "glm/glm.hpp"
#include <memory>
#include <cstdlib>

class Renderer
{
public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void Render(const Camera& camera, const Scene& scene);


private:     // Rendering //
    glm::vec4 PerPixel(uint32_t x, u_int32_t y);

    glm::vec4 RenderColor(Ray& ray, int depth);
    Payload TraceRay(const Ray& ray);
    Payload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
    Payload MissHit(const Ray& ray);

    uint32_t ConvertRGBA(glm::vec4 color);


private:        // Image //
    std::shared_ptr<Core::Image> m_FinalImage;
    uint32_t* m_imageData = nullptr;

private:        // Scene //
    const Scene* m_activeScene = nullptr;
    const Camera* m_activeCamera = nullptr;

public:    // Getter Functions //
    std::shared_ptr<Core::Image> getFinalImage() const { return m_FinalImage; }
    
    glm::vec3 GetLightDirection() const { return lightDirection; }
    int GetSamplesPerPixel() const { return samplesPerPixel; }
    int GetBounceDepth() const { return bounceDepth; }

    bool GetGiTag() const { return doGI; }
    bool GetRenderMode() const { return renderEachFrame; }


public:      // Exposable UI //
    static glm::vec3 lightDirection;
    static int samplesPerPixel;
    static int bounceDepth;
    static bool doGI;
    static bool renderEachFrame;
};