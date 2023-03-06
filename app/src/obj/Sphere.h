#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "Object.h"

class Sphere : public Object
{
public:
    Sphere() : Object() {};
    ~Sphere() override = default;

    Sphere(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
           bool pVisibility, bool pInReflections, float pRadius);

    bool intersect(const Ray &ray, tHit &quadratic) const override;
    bool intersectBB(AABB& outputBox) const override;

    bool solveQuadratic(const float &a, const float &b, const float &c, tHit &quadratic) const;

    // Getters
    float getSphereRadius() const { return radius; }
    void getUV(const glm::vec3& p, float& u, float& v) override;
    void getUI() override;

public:
    float radius;
};

#endif //RAYTRACING_SPHERE_H
