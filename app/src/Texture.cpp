#include "Texture.h"

glm::vec3 CheckerTexture::value(float u, float v, const glm::vec3 p) const
{
    float sines = glm::sin(20.0f * p.x) *
                  glm::sin(20.0f * p.y) *
                  glm::sin(20.0f * p.z);

    if (sines < 0) {
        return m_Odd->value(u, v, p);
    } else {
        return m_Even->value(u, v, p);
    }
}
