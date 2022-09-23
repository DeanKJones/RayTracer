#pragma once

#include "glm/glm.hpp"
#include <vector>

class Object
{
public: 
    Object() {};
    virtual ~Object() {};

    virtual bool intersect(const glm::vec3 &, const glm::vec3 &, float &) const = 0;
    virtual void getSurfaceData(const glm::vec3 &, glm::vec3 &, glm::vec3 &) const = 0;

// Create Object IDs
protected:
    static int nextID;
public:
    int ID;
};

class Sphere : public Object
{
public:
    Sphere() {};
    ~Sphere() {};

    Sphere(glm::vec3 pPos, float pRadius, glm::vec3 pColor);

    bool intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const;
    void getSurfaceData(const glm::vec3 &hitPosition, glm::vec3 &normal, glm::vec3 &surfaceColor) const;
    bool solveQuadratic(const float &a, const float &b, const float &c, float &t) const;

    // Getters
    glm::vec3 getSpherePosition() const { return sphereCenter; }
    float getSphereRadius() const { return radius; }
    glm::vec3 getSphereColor() const { return color[ID]; }

public:
    glm::vec3 sphereCenter;
    float radius;
    static std::vector<glm::vec3> color;
};
