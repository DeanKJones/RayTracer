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

    bool frontFace;
    inline void setFaceNormal(const Ray& ray, const glm::vec3 outwardNormal)
    {
        frontFace = glm::dot(ray.Direction, outwardNormal) < 0;
        worldNormal = frontFace ? outwardNormal : -outwardNormal;
    };

    std::shared_ptr<Material> materialPtr;

    int objectIndex;
};