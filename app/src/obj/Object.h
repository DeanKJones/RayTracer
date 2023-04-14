#pragma once

#include "glm/glm.hpp"

#include "Hit.h"

#include "bvh/AABB.h"

#include "../Ray.h"
#include "../Payload.h"
#include "../Material.h"

class Object
{
public: 
    Object() = default;
    virtual ~Object() = default;

    Object(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
           bool pVisibility, bool pInReflections) :
                objectName(pName), position(pPosition), material_ptr(pMaterial),
                isVisible(pVisibility), inReflections(pInReflections) {}

    virtual bool intersect(const Ray &, tHit &, Payload &) const = 0;
    virtual bool boundingBox(AABB& ) const = 0;

    virtual void getUI() = 0;
    virtual void getUV(const glm::vec3& , float& , float& ) const = 0;

    // Getters
    std::shared_ptr<Material> getMaterialPtr() const { return material_ptr; }

public:
    std::string objectName;
    glm::vec3 position;
    std::shared_ptr<Material> material_ptr;

    bool isVisible        = true;
    bool inReflections    = true;
};
