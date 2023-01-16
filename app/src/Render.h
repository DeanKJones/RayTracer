#pragma once

#include "Pixel.h"

#include "Camera.h"
#include "Scene.h"
#include "Material.h"

#include "Payload.h"

#include <memory>
#include <cstdlib>

class Renderer
{
public:
    struct Settings
    {
        bool accumulate = true;
        bool doGI = true;
        bool renderEachFrame = false; // default false to avoid crash on start up
        bool renderNormals = false;
        bool renderFacingRatio = false;
        bool renderSinglePixel = false;

        int  samples = 35;
        int  bounceDepth = 5;
        int  threadCount = 1;
    };

public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void Render(const Camera& camera, const Scene& scene);

    void ResetFrameIndex() { m_frameIndex = 1; }

    // Exposed for the magnified pixel to use
    Pixel PerPixel(uint32_t x, u_int32_t y);

private:     // Rendering //

    Pixel RenderColor(Ray& ray, int depth);

    Payload TraceRay(const Ray& ray);
    Payload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
    Payload MissHit(const Ray& ray);

    uint32_t ConvertRGBA(glm::vec4 color);


private:        // Image //
    std::shared_ptr<Core::Image> m_FinalImage;
    Settings m_settings;

    std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

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