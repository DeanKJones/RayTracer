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
    struct Settings
    {
        bool accumulate = false;
        int samples = 50;
        int bounceDepth = 5;
        bool doGI = false;
        bool renderEachFrame = true;
        bool renderNormals = false;
    };

public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void Render(const Camera& camera, const Scene& scene);

    void ResetFrameIndex() { m_frameIndex = 1; }

private:     // Rendering //
    glm::vec4 PerPixel(uint32_t x, u_int32_t y);

    glm::vec3 RenderColor(Ray& ray, int depth);
    Payload TraceRay(const Ray& ray);
    Payload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
    Payload MissHit(const Ray& ray);

    uint32_t ConvertRGBA(glm::vec4 color);


private:        // Image //
    std::shared_ptr<Core::Image> m_FinalImage;
    Settings m_settings;

    uint32_t* m_imageData = nullptr;
    glm::vec4* m_accumulationData = nullptr;

    uint32_t m_frameIndex = 1;

private:        // Scene //
    const Scene* m_activeScene = nullptr;
    const Camera* m_activeCamera = nullptr;


public:    // Getter Functions //
    std::shared_ptr<Core::Image> getFinalImage() const { return m_FinalImage; }
    Settings& GetSettings() { return m_settings; }
    uint32_t GetFrameIndex() const { return m_frameIndex; }
};