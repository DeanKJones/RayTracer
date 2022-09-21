#include "Render.h"
#include <iostream>

// UI VARIABLES
glm::vec3 Renderer::lightDirection  = {-1.0f, -1.0f, -1.0f};

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
void Renderer::Render(const Camera& camera, const std::vector<std::unique_ptr<Object>> &objects)
{
    Ray ray;
	ray.Origin = camera.GetPosition();
    
    const Object *hitObject = nullptr;

    // Aspect Ratio
    float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
        {    
            ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			glm::vec4 color = TraceRay(ray, objects);

            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

            // Image Data Pixel Position
            int p_pos = x + y * m_FinalImage->GetWidth();
            m_imageData[p_pos] = ConvertRGBA(color);
        }
    }
    m_FinalImage->SetData(m_imageData);
}

// Shader
glm::vec4 Renderer::TraceRay(const Ray& ray, const std::vector<std::unique_ptr<Object>> &objects)
{   
    std::vector<std::unique_ptr<Object>>::const_iterator iter = objects.begin();

    for (; iter != objects.end(); ++iter) {
        float t = INFINITY;
        if((*iter)->intersect(ray.Origin, ray.Direction, t)) {
            //hitObject = iter->get();
            //tNear = t;
            return RenderColor(ray, t, iter->get());
        } 
    } 
    return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 Renderer::RenderColor(const Ray& ray, float &tNear, Object *hitObject) 
{  
    glm::vec4 hitColor(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 hitPosition = ray.Origin + ray.Direction * tNear;
    glm::vec3 hitNormal;
    hitObject->getSurfaceData(hitPosition, hitNormal);

    // Create Light
    glm::vec3 lightDirection = GetLightDirection();
    glm::normalize(lightDirection);

    // Get light hits
    float light = glm::max(glm::dot(hitNormal, -lightDirection), 0.0f);
    // Base Color
    glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
    // Normals
    glm::vec3 colorNormals(hitNormal * 0.5f + 0.5f);
    // Color Lit
    glm::vec3 colorLit(objectColor * light);

    // Return hit object
    hitColor = glm::vec4(hitNormal, 1.0f);
 
    return hitColor;
}

// Color to Uint32_t
uint32_t Renderer::ConvertRGBA(glm::vec4 color) {
    uint32_t result = 
        ((uint8_t)(color.a * 255.0f) << 24) |
        ((uint8_t)(color.b * 255.0f) << 16) |
        ((uint8_t)(color.g * 255.0f) << 8)  |
        ((uint8_t)(color.r * 255.0f));
    return result;
}