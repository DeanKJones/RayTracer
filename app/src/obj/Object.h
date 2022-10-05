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
    //virtual std::shared_ptr<Material> getSurfaceData(Payload &) const = 0;
};

class Sphere : public Object
{
public:
    Sphere() {};
    ~Sphere() {};

    Sphere(glm::vec3 pPos, float pRadius, glm::vec3 pColor, std::shared_ptr<Material> material);

    bool intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const;
    //std::shared_ptr<Material> getSurfaceData() const;
    bool solveQuadratic(const float &a, const float &b, const float &c, float &t) const;

    // Getters
    glm::vec3 getSpherePosition() const { return position; }
    float getSphereRadius() const { return radius; }
    glm::vec3 getSphereColor() const { return albedo; }
    std::shared_ptr<Material> getMaterialPtr() const { return material_ptr; }

public:
    glm::vec3 position;
    float radius;
    glm::vec3 albedo;
    std::shared_ptr<Material> material_ptr;
};
