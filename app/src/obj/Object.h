#pragma once

#include "glm/glm.hpp"
#include "../Material.h"
#include <vector>
#include "../Payload.h"

class Object
{
public: 
    Object() {};
    virtual ~Object() {};

    

    virtual bool intersect(const glm::vec3 &, const glm::vec3 &, float &) const = 0;

    std::string objectName;
};

class Sphere : public Object
{
public:
    Sphere() : Object() {};
    ~Sphere() {};

    Sphere(glm::vec3 pPos, float pRadius, std::shared_ptr<Material> material);

    bool intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const override;
    bool solveQuadratic(const float &a, const float &b, const float &c, float &t) const;

    // Getters
    glm::vec3 getSpherePosition() const { return position; }
    float getSphereRadius() const { return radius; }
    std::shared_ptr<Material> getMaterialPtr() const { return material_ptr; }

public:
    glm::vec3 position;
    float radius;
    std::shared_ptr<Material> material_ptr;
};
