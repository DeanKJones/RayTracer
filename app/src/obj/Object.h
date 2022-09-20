#pragma once

#include "glm/glm.hpp"

class Object
{
public: 
    Object() {};
    virtual ~Object() {};

    virtual bool intersect(const glm::vec3 &, const glm::vec3 &, float &) const = 0;
    virtual void getSurfaceData(const glm::vec3 &, glm::vec3 &) const = 0;
};

class Sphere : public Object
{
public:
    Sphere() {};
    ~Sphere() {};

    Sphere(glm::vec3 pPos, float pRadius, glm::vec3 pColor);

    bool intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const;
    void getSurfaceData(const glm::vec3 &position, glm::vec3 &normal) const;
    bool solveQuadratic(const float &a, const float &b, const float &c, float &t) const;

    // Getters
    glm::vec3 getSpherePosition() { return sphereCenter; }
    float getSphereRadius() { return radius; }
    glm::vec3 getSphereColor() { return color; }

public:
    static glm::vec3 sphereCenter;
    static float radius;
    static glm::vec3 color;
};
