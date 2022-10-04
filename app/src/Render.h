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


private:        // Rendering //
    // Payload struct to set and grab information from the scene
    struct Payload 
    {
        float hitDistance;
        glm::vec3 worldPosition;
        glm::vec3 worldNormal;
        glm::vec3 surfaceColor;

        int objectIndex;
    };

    glm::vec4 PerPixel(uint32_t x, u_int32_t y);

    glm::vec4 RenderColor(Ray& ray, int depth);
    Payload TraceRay(const Ray& ray);
    Payload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
    Payload MissHit(const Ray& ray);

    uint32_t ConvertRGBA(glm::vec4 color, int& samples);


private:        // Image //
    std::shared_ptr<Core::Image> m_FinalImage;
    uint32_t* m_imageData = nullptr;

private:        // Scene //
    const Scene* m_activeScene = nullptr;
    const Camera* m_activeCamera = nullptr;

public:         // Getter Functions //
    std::shared_ptr<Core::Image> getFinalImage() const { return m_FinalImage; }
    
    glm::vec3 GetLightDirection() const { return lightDirection; }
    int GetSamplesPerPixel() const { return samplesPerPixel; }
    int GetBounceDepth() const { return bounceDepth; }

    bool GetGiTag() const { return doGI; }
    bool GetRenderMode() const { return renderEachFrame; }
    bool GetLambertMethod() const { return lambertMethod; }


public:         // Exposable UI //
    static glm::vec3 lightDirection;
    static int samplesPerPixel;
    static int bounceDepth;
    static bool doGI;
    static bool renderEachFrame;
    static bool lambertMethod;
};