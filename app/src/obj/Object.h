#pragma once

#include "glm/glm.hpp"
#include "../Material.h"
#include <vector>

class Object
{
public: 
    Object() {};
    virtual ~Object() {};

    Object(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial) :
            objectName(pName), position(pPosition), material_ptr(pMaterial) {}

    virtual bool intersect(const glm::vec3 &, const glm::vec3 &, float &) const = 0;

    // Getters
    glm::vec3 getObjectPosition() const { return position; }
    std::shared_ptr<Material> getMaterialPtr() const { return material_ptr; }

public:
    std::string objectName;
    glm::vec3 position;
    std::shared_ptr<Material> material_ptr;
};
