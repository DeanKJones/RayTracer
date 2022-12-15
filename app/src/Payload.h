#include <iostream>

#include "glm/glm.hpp"
#include "Material.h"

// Payload struct to set and grab information from the scene
struct Payload 
{
    float hitDistance;
    glm::vec3 hitPosition;
    glm::vec3 worldNormal;

    bool frontFace;
    inline void setFaceNormal(const glm::vec3& direction, const glm::vec3& outwardNormal) {
        frontFace = glm::dot(direction, outwardNormal) < 0;
        worldNormal = frontFace ? outwardNormal : -outwardNormal;
    }

    std::shared_ptr<Material> materialPtr;
    int objectIndex;
};