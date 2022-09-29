#include "Render.h"
#include <iostream>

// UI VARIABLES
glm::vec3 Renderer::lightDirection  = {1.0f, -1.0f, -1.0f};
int Renderer::samplesPerPixel = 15.0f;
int Renderer::bounceDepth = 10.0f;
bool Renderer::doGI = false;
bool Renderer::renderEachFrame = false;


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
    Ray ray;
    const Object *hitObject = nullptr;
    
    // Aspect Ratio
    float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
        {    
            int p_pos;
            glm::vec4 color(0.0f, 0.0f, 0.0f, 0.0f);

            int samplesPerPixel = GetSamplesPerPixel();
            if (samplesPerPixel < 1)
                samplesPerPixel = 1;

            for (uint32_t s = 0; s < samplesPerPixel; ++s){

                if (renderEachFrame){
                    samplesPerPixel = 1;
                }

                // Set ray origin back to camera after ray bounces around the scene
                ray.Origin = camera.GetPosition();
                ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

                // Scatter Rays
                ray.Direction.x += (Core::Random::Float() * 0.0001f);
                ray.Direction.y += (Core::Random::Float() * 0.0001f);

                int depth = GetBounceDepth();
                // Render Color
                color += RenderColor(ray, scene, depth);
                //color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

                // Image Data Pixel Position
                p_pos = x + y * m_FinalImage->GetWidth();
            }
            m_imageData[p_pos] = ConvertRGBA(color, samplesPerPixel);
        }
    }
    m_FinalImage->SetData(m_imageData);
}

// Shader
bool Renderer::TraceRay(const Ray& ray, const Scene& scene, float &tNear, const Object *&hitObject)
{   
    tNear = MAXFLOAT;

    for (const Sphere& sphere : scene.spheres) {
        float t = MAXFLOAT;
        if(sphere.intersect(ray.Origin, ray.Direction, t) && t < tNear) {
            hitObject = &sphere;
            tNear = t;
        } 
    } 
    return (hitObject != nullptr);
}

glm::vec4 Renderer::RenderColor(Ray& ray, const Scene& scene, int depth) 
{  
    if (depth <= 0){
        return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    // Set Colors
    glm::vec3 unitVector = glm::normalize(ray.Direction);
    float t = 0.5 * (unitVector.y + 1.0f);
    glm::vec4 RayHitColor(((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)), 1.0f);

    // Create object and hit distance
    float tNear;
    const Object *hitObject = nullptr;

    if(TraceRay(ray, scene, tNear, hitObject)) {
        
        // Set variables to grab surface data
        glm::vec3 hitPosition = ray.Origin + ray.Direction * tNear;
        glm::vec3 hitNormal;
        glm::vec3 hitColor;

        hitObject->getSurfaceData(hitPosition, hitNormal, hitColor);

        // Create Light
        glm::vec3 lightDirection = GetLightDirection();
        glm::normalize(lightDirection);
        // Get light hits
        float light = glm::max(glm::dot(hitNormal, -lightDirection), 0.0f);

        // Normals
        glm::vec3 colorNormals(hitNormal * 0.5f + 0.5f);
        // Normal Colors
        glm::vec4 objectNormal(colorNormals, 1.0f);
        // Color Lit
        glm::vec3 colorLit(hitColor * light);

        // Return object color
        RayHitColor = glm::vec4(colorLit, 1.0f);

        // Do GI check before rendering
        bool GI = GetGiTag();
        bool eachFrame = GetRenderMode();
        if(GI && !eachFrame){
            // Set new random position for ray bounces
            glm::vec3 newRayTarget = hitPosition + hitNormal + Core::Random::InUnitSphere();
            ray.Origin = hitPosition;
            ray.Direction = newRayTarget - hitPosition;
            auto rayBounce = RenderColor(ray, scene, depth -1);
            rayBounce *= 0.5f;
            return rayBounce;
        }
    }
    return RayHitColor;
}

// Color to Uint32_t
uint32_t Renderer::ConvertRGBA(glm::vec4 color, int& samples) 
{
    auto colorR = color.r;
    auto colorG = color.g;
    auto colorB = color.b;
    auto colorA = color.a;

    auto scale = 1.0 / samples;
    colorR = glm::sqrt(scale * colorR);
    colorG = glm::sqrt(scale * colorG);
    colorB = glm::sqrt(scale * colorB);

    uint32_t result = 
        ((uint8_t)(colorA * 255.0f) << 24) |
        ((uint8_t)(glm::clamp(colorB, 0.0f, 0.999f) * 255.0f) << 16) |
        ((uint8_t)(glm::clamp(colorG, 0.0f, 0.999f) * 255.0f) << 8)  |
        ((uint8_t)(glm::clamp(colorR, 0.0f, 0.999f) * 255.0f));
    return result;
}