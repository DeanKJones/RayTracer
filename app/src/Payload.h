#include <iostream>

#include "glm/glm.hpp"
#include "Material.h"

// Payload struct to set and grab information from the scene
struct Payload 
{
    float hitDistance;
    glm::vec3 worldPosition;
    glm::vec3 worldNormal;

    static bool frontFace(const glm::vec3& direction, const glm::vec3& normal) {
        if (glm::dot(direction, normal) < 0)
            return true;
        else
            return false;
    }

    std::shared_ptr<Material> materialPtr;

    int objectIndex;
};