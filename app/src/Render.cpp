#include "Render.h"


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
void Renderer::Render()
{
    // Aspect Ratio
    float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
        {    
            glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
            coord = coord * 2.0f - 1.0f;  // -1 -> 1
            // Aspect Ratio
            coord.x *= aspectRatio;
            // Color
            glm::vec4 color = perPixel(coord);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            // Image Data Pixel Position
            int p_pos = x + y * m_FinalImage->GetWidth();
            m_imageData[p_pos] = ConvertRGBA(color);
        }
    }
    m_FinalImage->SetData(m_imageData);
}

// Shader
glm::vec4 Renderer::perPixel(glm::vec2 coord)
{   
    // Create Sphere
    float radius = 0.5f;
    glm::vec3 spherePos(0.0f, 0.0f, 0.0f);
    createSphere(spherePos, radius);

    glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    glm::vec3 rayDirection(coord.x, coord.y, -1.0f);

    // (bx^2 + bx^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // Where: 
    // a = Ray Origin
    // b = Ray Direction
    // r = Radius
    // t = Hit Distance

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot((rayOrigin - spherePos), rayDirection);  // rayOrigin - spherePosition allows the sphere to move
    float c = glm::dot(rayOrigin, rayOrigin) - s_radius * s_radius;

    /*  Quadratic Formula
    *         ___________
    *   -b +-/ b^2 - 4ac
    *           2a
    */  

    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f){
        return glm::vec4({0.0f, 0.0f, 0.0f, 1.0f});
    }

    // Quadtratic solutions
    float rayHit = (-b - glm::sqrt(discriminant)) / (2.0f * a);
    // float HitDistant = (-b + glm::sqrt(discriminant)) / (2.0f * a);

    // Create Light
    glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);
    glm::normalize(lightDirection);

    // For both quadratic solutions
    for (int i = 0; i < 2; i++) {
        glm::vec3 hitPosition = rayOrigin + rayDirection * rayHit;
        glm::vec3 normal = glm::normalize(hitPosition - spherePos);

        // Get light hits
        float light = glm::max(glm::dot(normal, -lightDirection), 0.0f);
        // Solid Unlit
        glm::vec3 sphereColor(1.0f, 1.0f, 1.0f);
        // Normals
        glm::vec3 colorNormals(normal * 0.5f + 0.5f);
        // Color Lit
        glm::vec3 colorLit(sphereColor * light);

        // Return hit object
        return glm::vec4(colorLit, 1.0f);
    }
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

// Sphere 
void Renderer::createSphere(glm::vec3 position, float radius) {
    s_pos = position;
    s_radius = radius;
}