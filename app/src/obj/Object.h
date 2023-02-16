#pragma once

#include "glm/glm.hpp"
#include "../Material.h"
#include <vector>

class Object
{
public:
    struct tHit
    {
        float t_near;
        float t_far;
    };

public: 
    Object() {};
    virtual ~Object() {};

    Object(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
           bool pVisibility, bool pInReflections) :
                objectName(pName), position(pPosition), material_ptr(pMaterial),
                isVisible(pVisibility), inReflections(pInReflections) {}

    virtual bool intersect(const glm::vec3 &, const glm::vec3 &, tHit &) const = 0;
    virtual void getUI() = 0;

    // Getters
    glm::vec3 getObjectPosition() const { return position; }
    std::shared_ptr<Material> getMaterialPtr() const { return material_ptr; }
    tHit& getIntersector() { return intersector; }

public:
    std::string objectName;
    glm::vec3 position;
    std::shared_ptr<Material> material_ptr;

    bool isVisible;
    bool inReflections;

    tHit intersector;
};
