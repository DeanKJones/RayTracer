#include "Render.h"
#include "imgui.h"

#include <random>

#include "algorithm"
#include "../../core/external/libomp/include/omp.h"

using namespace Walnut;

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
        m_FinalImage = std::make_shared<Image>(width, height, ImageFormat::RGBA);
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
    std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
                  [this](uint32_t y)
    {
        std::for_each(td::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
                  [this, y](uint32_t x)
        {
            Pixel pixel = PerPixel(x, y);
            m_accumulationData[x + y * m_FinalImage->GetWidth()] += pixel.RGBA;

            Pixel accumulatedColor;
            accumulatedColor.RGBA = m_accumulationData[x + y * m_FinalImage->GetWidth()];
            accumulatedColor.RGBA /= (float)m_frameIndex;
            accumulatedColor.RGBA = glm::clamp(accumulatedColor.RGBA, glm::vec4(0.0f), glm::vec4(1.0f));

            // Image Data Pixel Position
            m_imageData[x + y * m_FinalImage->GetWidth()] = ConvertRGBA(accumulatedColor.RGBA);
        });
    });


    //
    // #pragma omp parallel for schedule(dynamic, 1)
    //

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
    if (!m_settings.renderSinglePixel){
        ray.Direction += Walnut::Random::Vec3(-0.00005f, 0.00005f);
    }
    // Normalize Direction
    glm::normalize(ray.Direction);

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

    if (depth <= 0) {
        pixel.RGB += RayHitColor;
        return pixel;
    }

    if (depth == m_settings.bounceDepth){
        ray.isFirstBounce = true;
    } else {
        ray.isFirstBounce = false;
    }

    // Load the weapon and trace the ray
    Payload payload = TraceRay(ray);

    // No Hit -> Return Sky
    if (payload.hitDistance < 0){
        // Set Colors
        glm::vec3 unitVector = glm::normalize(ray.Direction);
        float t = 0.5 * (unitVector.y + 1.0f);
        glm::vec3 SkyColor(((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)));

        // Catch the final ray for RayToLine
        if (m_settings.renderSinglePixel) {
            ray.HitDistance   = payload.hitDistance;
            m_activeScene->rayToLine.push_back(ray);
        }

        pixel.RGB = SkyColor;
        return pixel;
    }

    // Add Ray to vector
    // Done here to avoid duplicate rays from being pushed back
    if (m_settings.renderSinglePixel) {
        m_activeScene->rayToLine.push_back(ray);
    }

    if (payload.objectType == "Line")
    {
        depth = 0;

        RayHitColor = payload.materialPtr->albedo->value(payload.u, payload.v, payload.hitPosition);
        pixel.RGB = RayHitColor;
        return pixel;
    }

    // Do GI check before rendering
    bool GI = m_settings.doGI;
    if(GI)
    {
        Ray scatteredRay;
        glm::vec3 attenuation;

        if (payload.materialPtr->scatter(ray, payload, attenuation, scatteredRay))
        {
            Pixel bounced = RenderColor(scatteredRay, depth - 1);
            pixel.RGB = (attenuation * bounced.RGB) * 0.75f;
            return pixel;
        }
        pixel.RGB = {0.0f, 0.0f, 0.0f};
        return pixel;
    }

    // Debug views
    if (m_settings.renderNormals){
        glm::vec3 colorNormals(payload.worldNormal * 0.5f + 0.5f);
        pixel.RGB = colorNormals;
    }
    else if (m_settings.renderFacingRatio){
        glm::vec3 FacingRatio(glm::dot(payload.worldNormal, -ray.Direction));
        pixel.RGB = FacingRatio;
    }
    else {
        glm::vec3 colorAlbedo(payload.materialPtr->albedo->value(payload.u, payload.v, payload.hitPosition));
        pixel.RGB = colorAlbedo;
    }
    return pixel;
}

// Shader
Payload Renderer::TraceRay(const Ray& ray)
{
    tHit intersector;
    intersector.t_near = 0.00001f;
    intersector.t_far  = std::numeric_limits<float>::max();

    Payload payload;

    // Use the scene intersect function
    if (m_activeScene->intersect(ray, intersector, payload)){
        return payload;
    } else {
        return MissHit();
    }
}

Payload Renderer::MissHit()
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

void Renderer::GetUI()
{
    ImGui::Text("Turn on Light Bouncing: ");
    ImGui::Checkbox(": GI", &GetSettings().doGI);

    ImGui::Text("Accumulate samples over time: ");
    ImGui::Checkbox(": Accumulate", &GetSettings().accumulate);

    ImGui::Text("Use Lambert Hemisphere Model: ");
    ImGui::Checkbox(": Scattering Type", (bool*)&Lambertian::lambertHemi);
    ImGui::Separator();
    ImGui::Checkbox(": Display Normals", &GetSettings().renderNormals);
    ImGui::Checkbox(": Facing Ratio", &GetSettings().renderFacingRatio);

    ImGui::Separator();

    // Samples per pixel lock at 1, values below 1 will be ignored
    ImGui::Text("Maximum number of samples: ");
    ImGui::InputInt(": Samples", &GetSettings().samples);

    ImGui::Text("Ray Bounce Depth: ");
    ImGui::InputInt(": Ray Bounces", &GetSettings().bounceDepth);
}