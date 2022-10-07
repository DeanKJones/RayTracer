#include "Render.h"
#include <random>

// UI VARIABLES
glm::vec3 Renderer::lightDirection  = {1.0f, -1.0f, -1.0f};
int Renderer::samplesPerPixel = 10.0f;
int Renderer::bounceDepth = 3.0f;
bool Renderer::doGI = false;
bool Renderer::renderEachFrame = false;
bool Renderer::lambertMethod = false;


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
}

// Render
void Renderer::Render(const Camera& camera, const Scene& scene)
{
    m_activeCamera = &camera;
    m_activeScene = &scene;
    
    // Aspect Ratio
    float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
    int pixel_pos;

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
        {    
            glm::vec4 color = PerPixel(x, y);

            // Image Data Pixel Position
            pixel_pos = x + y * m_FinalImage->GetWidth();
            m_imageData[pixel_pos] = ConvertRGBA(color);
        }
    }
    m_FinalImage->SetData(m_imageData);
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
    Ray ray;
    // Set ray origin back to camera after ray bounces around the scene
    ray.Origin = m_activeCamera->GetPosition();
    ray.Direction = m_activeCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

    glm::vec4 color(0.0f, 0.0f, 0.0f, 0.0f);
    float multiplier = 1.0f;

    int samplesPerPixel = GetSamplesPerPixel();
    if (samplesPerPixel < 1)
        samplesPerPixel = 1;

    for (uint32_t s = 0; s < samplesPerPixel; ++s)
    {
        if (renderEachFrame){
            samplesPerPixel = 1;
        }
        // Scatter Rays
        ray.Direction.x += (Core::Random::Float() * 0.0001f);
        ray.Direction.y += (Core::Random::Float() * 0.0001f);

        int depth = GetBounceDepth();
        // Render Color
        color += RenderColor(ray, depth, multiplier);
    }
    return color;
}


glm::vec4 Renderer::RenderColor(Ray& ray, int depth, float multiplier) 
{  
    if (depth <= 0){
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Set Colors
    glm::vec4 RayHitColor(0.0f);
    // Load the weapon and trace ray
    Payload payload = TraceRay(ray);

    if (payload.hitDistance < 0){
        // Set Colors
        glm::vec3 unitVector = glm::normalize(ray.Direction);
        float t = 0.5 * (unitVector.y + 1.0f);
        glm::vec4 SkyColor(((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)), 1.0f);

        RayHitColor += SkyColor;
        return RayHitColor;
    }

    // Do GI check before rendering
    bool GI = GetGiTag();
    bool eachFrame = GetRenderMode();
    bool lambertMethod = GetLambertMethod();

    if(GI && !eachFrame){
        Ray scattered;
        glm::vec3 attenuation;

        if (payload.materialPtr->scatter(ray, payload, attenuation, scattered))
        {
            glm::vec4 attenColor(attenuation, 1.0f);
            glm::vec4 bounceColor = (attenColor * RenderColor(scattered, depth - 1, multiplier)) * multiplier;
            multiplier *= 0.7f;
            return bounceColor;
        }
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    // Normals
    glm::vec3 colorNormals(payload.worldNormal * 0.5f + 0.5f);
    // Return object color
    RayHitColor = glm::vec4(colorNormals, 1.0f);

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

    int samples = GetSamplesPerPixel();
    auto scale = 1.0 / samples;
    colorR = glm::sqrt(scale * colorR);
    colorG = glm::sqrt(scale * colorG);
    colorB = glm::sqrt(scale * colorB);

    // colorR = glm::sqrt(colorR);
    // colorG = glm::sqrt(colorG);
    // colorB = glm::sqrt(colorB);

    uint32_t result = 
        ((uint8_t)(colorA * 255.0f) << 24) |
        ((uint8_t)(glm::clamp(colorB, 0.0f, 0.999f) * 255.0f) << 16) |
        ((uint8_t)(glm::clamp(colorG, 0.0f, 0.999f) * 255.0f) << 8)  |
        ((uint8_t)(glm::clamp(colorR, 0.0f, 0.999f) * 255.0f));
    return result;
}