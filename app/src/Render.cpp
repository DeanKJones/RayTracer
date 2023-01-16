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

#define MT 0
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
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
            Pixel pixel = PerPixel(x, y);
            m_accumulationData[x + y * m_FinalImage->GetWidth()] += pixel.RGBA;

            Pixel accumulatedPixel;
            accumulatedPixel.RGBA = m_accumulationData[x + y * m_FinalImage->GetWidth()];
            accumulatedPixel.RGBA /= (float) m_frameIndex;

            accumulatedPixel.RGBA = glm::clamp(accumulatedPixel.RGBA, glm::vec4(0.0f), glm::vec4(1.0f));
            // Image Data Pixel Position
            m_imageData[x + y * m_FinalImage->GetWidth()] = ConvertRGBA(accumulatedPixel.RGBA);
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

Pixel Renderer::PerPixel(uint32_t x, uint32_t y)
{
    Ray ray;
    Pixel pixel;

    // Set ray origin back to camera after ray bounces around the scene
    ray.Origin = m_activeCamera->GetPosition();
    ray.Direction = m_activeCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

    // Set Pixel Color with the Alpha at 1
    pixel.RGBA = {0.0f, 0.0f, 0.0f, 1.0f};

    // Scatter Rays
    ray.Direction += Core::Random::Vec3(-0.0005f, 0.0005f);

    int depth = m_settings.bounceDepth;

    // Render Color
    Pixel renderedPixel = RenderColor(ray, depth);
    pixel.RGBA = glm::vec4(renderedPixel.RGB, 1.0f);

    return pixel;
}


Pixel Renderer::RenderColor(Ray& ray, int depth)
{
    // Set Colors
    Pixel pixel;
    glm::vec3 RayHitColor(0.0f, 0.0f, 0.0f);

    if (depth <= 0){
        if (m_settings.renderSinglePixel){
            // Get the last bounce of the ray
            m_activeScene->rayToLine.push_back(ray);
        }
        pixel.RGB += RayHitColor;
        return pixel;
    }

    if (depth == m_settings.bounceDepth){
        ray.isFirstBounce = true;
    } else {
        ray.isFirstBounce = false;
    }

    // Add ray to the start of the rayToLine vector
    if (m_settings.renderSinglePixel){
        m_activeScene->rayToLine.push_back(ray);
    }
    // Load the weapon and trace the ray
    Payload payload = TraceRay(ray);

    // No Hit -> Return Sky
    if (payload.hitDistance < 0){
        // Set Colors
        glm::vec3 unitVector = glm::normalize(ray.Direction);
        float t = 0.5 * (unitVector.y + 1.0f);
        glm::vec3 SkyColor(((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)));

        pixel.RGB = SkyColor;
        return pixel;
    }

    if (payload.objectType == "Line")
    {
        depth = 0;

        RayHitColor = payload.materialPtr->albedo;
        pixel.RGB = RayHitColor;
        return pixel;
    }

    // Do GI check before rendering
    bool GI = m_settings.doGI;
    if(GI){
        Ray scatteredRay;
        glm::vec3 attenuation;

        if (payload.materialPtr->scatter(ray, payload, attenuation, scatteredRay))
        {
            Pixel bounced = RenderColor(scatteredRay, depth - 1);
            pixel.RGB = (attenuation * bounced.RGB) * 0.75f;
            if (m_settings.renderSinglePixel)
            {
                std::cout << "Ray Position: " << scatteredRay.Origin.x << ", "
                                              << scatteredRay.Origin.y << ", "
                                              << scatteredRay.Origin.z << "\n";
            }
            return pixel;
        }
        return pixel;
    }

    if (m_settings.renderNormals){
        glm::vec3 colorNormals(payload.worldNormal * 0.5f + 0.5f);
        pixel.RGB = colorNormals;
    }
    else if (m_settings.renderFacingRatio){
        glm::vec3 FacingRatio(glm::dot(payload.worldNormal, -ray.Direction));
        pixel.RGB = FacingRatio;
    }
    else {
        glm::vec3 colorAlbedo(payload.materialPtr->albedo);
        pixel.RGB = colorAlbedo;
    }
    return pixel;
}

// Shader
Payload Renderer::TraceRay(const Ray& ray)
{   
    int closest_T = -1;
    float hitDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < m_activeScene->sceneObjects.size(); i++) {
        float t = MAXFLOAT;
        Object* object = m_activeScene->sceneObjects[i];
        if (!object->isVisible)
            continue;

        // Way of keeping our Line from rendering in reflections
        if (!ray.isFirstBounce && !object->inReflections) {
            continue;
        }

        if (object->intersect(ray.Origin, ray.Direction, t) && t < hitDistance)
        {
            hitDistance = t;
            closest_T = (int)i;
        } 
    } 
    if (closest_T < 0){
        return MissHit(ray);
    }
    return ClosestHit(ray, hitDistance, closest_T);
}


Payload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
    Payload payload;
    payload.hitDistance = hitDistance;
    payload.objectIndex = objectIndex;

    Object* object = m_activeScene->sceneObjects[objectIndex];

    std::string typeidName = typeid(*(object)).name();

    if (typeidName.find("Sphere") != std::string::npos)
    {
        payload.hitPosition = ray.at(hitDistance);
        glm::vec3 outwardNormal = (payload.hitPosition - object->position) / reinterpret_cast<Sphere *>(object)->radius;
        payload.setFaceNormal(ray.Direction, outwardNormal);

        glm::vec3 origin = ray.Origin - object->position;
        payload.hitPosition = origin + ray.Direction * hitDistance;
        payload.materialPtr = object->getMaterialPtr();

        // Add sphere position back
        payload.hitPosition += object->position;
        payload.objectType = "Sphere";

        return payload;
    }
    else if (typeidName.find("Line") != std::string::npos)
    {
        payload.hitPosition = ray.at(hitDistance);
        payload.materialPtr = object->getMaterialPtr();
        payload.objectType  = "Line";

        return payload;
    }
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