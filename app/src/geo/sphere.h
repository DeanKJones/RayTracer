#pragma once

#include "glm/glm.hpp"

class Sphere
{
public:
    Sphere() = default;
    void createSphere(glm::vec3 position, float radius);

private:
    glm::vec3 m_pos;
    float m_radius;
};