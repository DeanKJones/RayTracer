#include <iostream>

#include "glm/glm.hpp"
#include "Material.h"

// Payload struct to set and grab information from the scene
struct Payload 
{
    float hitDistance;
    glm::vec3 worldPosition;
    glm::vec3 worldNormal;
    glm::vec3 surfaceColor;

    std::shared_ptr<Material> materialPtr;

    int objectIndex;
};