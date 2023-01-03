#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "Object.h"

class Sphere : public Object
{
public:
    Sphere() : Object() {};
    ~Sphere() {};

    Sphere(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial, float pRadius);

    bool intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const override;
    bool solveQuadratic(const float &a, const float &b, const float &c, float &t) const;

    // Getters
    float getSphereRadius() const { return radius; }

public:
    float radius;
};

#endif //RAYTRACING_SPHERE_H
