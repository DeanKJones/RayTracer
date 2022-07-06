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
    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
        {    
            glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
            coord = coord * 2.0f - 1.0f;  // -1 -> 1
            // Image Data Pixel Position
            int p_pos = x + y * m_FinalImage->GetWidth();
            m_imageData[p_pos] = perPixel(coord);
        }
    }
    m_FinalImage->SetData(m_imageData);
}

// Shader
uint32_t Renderer::perPixel(glm::vec2 coord)
{   
    float radius = 0.5f;
    


    glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
    rayDirection = glm::normalize(rayDirection);

    // (bx^2 + bx^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // Where: 
    // a = Ray Origin
    // b = Ray Direction
    // r = Radius
    // t = Hit Distance

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayOrigin, rayDirection);
    float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

    /*  Quadratic Formula
    *         ___________
    *   -b +-/ b^2 - 4ac
    *           2a
    */  

    float discriminant = b * b - 4.0f * a * c;

    if (discriminant >= 0.0f){
        return 0xff00ffff;
    }
    return 0xff000000;
}

// TODO:
//      Solve the rest of the quadratic formula 
//      Return the ray hit distances 
//      Find the 3D Coordinate of everything 
//      Apply some shading to it (normal or otherwise)

void Renderer::createSphere(glm::vec3 position, float radius) {
    s_pos = position;
    s_radius = radius;
}