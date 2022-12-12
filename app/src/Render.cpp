#include "Render.h"

#include <random>

void Renderer::onResize(uint32_t width, uint32_t height)
{
    if(m_FinalImage) {
        // No Resize 
        if(m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height){
            return;
        }
        m_FinalImage->Resize(width, height);
    }
    else {
        m_FinalImage = std::make_shared<Core::Image>(width, height, Core::ImageFormat::RGBA);
    }
    delete[] m_imageData;
    m_imageData = new uint32_t[width * height];

    delete[] m_accumulationData;
    m_accumulationData = new glm::vec4[width * height];

    m_ImageHorizontalIter.resize(width);
    m_ImageVerticalIter.resize(height);

    for (int32_t i = 0; i < width; i++)
        m_ImageHorizontalIter[i] = i;
    for (int32_t i = 0; i < height; i++)
        m_ImageVerticalIter[i] = i;
}

// Render
void Renderer::Render(const Camera& camera, const Scene& scene)
{
    m_activeCamera = &camera;
    m_activeScene = &scene;

    if (m_frameIndex == 1) {
        memset(m_accumulationData, 0,
               m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4));
    }

#define MT 1
#if MT

    std::for_each(m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
                  [this](uint32_t y)
    {
        std::for_each(m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
                  [this, y](uint32_t x)
        {
            glm::vec4 color = PerPixel(x, y);
            m_accumulationData[x + y * m_FinalImage->GetWidth()] += color;

            glm::vec4 accumulatedColor = m_accumulationData[x + y * m_FinalImage->GetWidth()];
            accumulatedColor /= (float)m_frameIndex;
            accumulatedColor = glm::clamp(accumulatedColor,glm::vec4(0.0f), glm::vec4(1.0f));

            // Image Data Pixel Position
            m_imageData[x + y * m_FinalImage->GetWidth()] = ConvertRGBA(accumulatedColor);
        });
    });

#else

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
        {
            glm::vec4 color = PerPixel(x, y);
            m_accumulationData[x + y * m_FinalImage->GetWidth()] += color;

            glm::vec4 accumulatedColor = m_accumulationData[x + y * m_FinalImage->GetWidth()];
            accumulatedColor /= (float)m_frameIndex;

            accumulatedColor = glm::clamp(accumulatedColor,glm::vec4(0.0f), glm::vec4(1.0f));
            // Image Data Pixel Position
            m_imageData[x + y * m_FinalImage->GetWidth()] = ConvertRGBA(accumulatedColor);
        }
    }

#endif

    m_FinalImage->SetData(m_imageData);

    if (m_settings.accumulate){
        m_frameIndex++;
    } else {
        m_frameIndex = 1;
    }
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
    Ray ray;
    // Set ray origin back to camera after ray bounces around the scene
    ray.Origin = m_activeCamera->GetPosition();
    ray.Direction = m_activeCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

    // Set Pixel Color with the Alpha at 1
    glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);

    // Scatter Rays
    ray.Direction += Core::Random::Vec3(-0.0005f, 0.0005f);

    int depth = m_settings.bounceDepth;
    // Render Color
    glm::vec3 renderedColor;
    renderedColor = RenderColor(ray, depth);
    color = glm::vec4(renderedColor, 1.0f);

    return color;
}


glm::vec3 Renderer::RenderColor(Ray& ray, int depth) 
{  
    // Set Colors
    glm::vec3 RayHitColor(0.0f, 0.0f, 0.0f);

    if (depth <= 0){
        return RayHitColor;
    }

    // Load the weapon and trace the ray
    Payload payload = TraceRay(ray);

    // No Hit -> Return Sky
    if (payload.hitDistance < 0){
        // Set Colors
        glm::vec3 unitVector = glm::normalize(ray.Direction);
        float t = 0.5 * (unitVector.y + 1.0f);
        glm::vec3 SkyColor(((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)));

        RayHitColor = SkyColor;
        return RayHitColor;
    }

    // Do GI check before rendering
    bool GI = m_settings.doGI;

    if(GI){
        Ray scatteredRay;
        glm::vec3 attenuation;

        if (payload.materialPtr->scatter(ray, payload, attenuation, scatteredRay))
        {
            RayHitColor = (attenuation * RenderColor(scatteredRay, depth - 1)) * 0.75f;
            return RayHitColor;
        }
        return RayHitColor;
    }

    if (m_settings.renderNormals){
        glm::vec3 colorNormals(payload.worldNormal * 0.5f + 0.5f);
        RayHitColor = colorNormals;
    } else {
        glm::vec3 colorAlbedo(payload.materialPtr->albedo);
        RayHitColor = colorAlbedo;
    }
    return RayHitColor;
}

// Shader
Payload Renderer::TraceRay(const Ray& ray)
{   
    int closestSphere = -1;
    float hitDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < m_activeScene->spheres.size(); i++) {
        float t = MAXFLOAT;
        Sphere activeSphere = m_activeScene->spheres[i];
        if (activeSphere.intersect(ray.Origin, ray.Direction, t) && t < hitDistance) {
            hitDistance = t;
            closestSphere = (int)i;
        } 
    } 
    if (closestSphere < 0){
        return MissHit(ray);
    }
    return ClosestHit(ray, hitDistance, closestSphere);
}


Payload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
    Payload payload;
    payload.hitDistance = hitDistance;
    payload.objectIndex = objectIndex;

    const Sphere& closestSphere = m_activeScene->spheres[objectIndex];

//    glm::vec3 outwardNormal = (payload.worldPosition - closestSphere.position) / closestSphere.radius;
//    payload.setFaceNormal(ray, outwardNormal);

    glm::vec3 origin = ray.Origin - closestSphere.position;
    payload.worldPosition = origin + ray.Direction * hitDistance;

    payload.worldNormal = glm::normalize(payload.worldPosition);

    payload.materialPtr = closestSphere.getMaterialPtr();
    // Add sphere position back
    payload.worldPosition += closestSphere.position;

    return payload;
}

Payload Renderer::MissHit(const Ray& ray)
{
    Payload payload;
	payload.hitDistance = -1.0f;
	return payload;
}

// Color to Uint32_t
uint32_t Renderer::ConvertRGBA(glm::vec4 color) 
{
    auto colorR = color.r;
    auto colorG = color.g;
    auto colorB = color.b;
    auto colorA = color.a;

    uint32_t result = 
        ((uint8_t)(colorA * 255.0f) << 24) |
        ((uint8_t)(colorB * 255.0f) << 16) |
        ((uint8_t)(colorG * 255.0f) << 8)  |
        ((uint8_t)(colorR * 255.0f));
    return result;
}