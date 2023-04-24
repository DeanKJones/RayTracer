#ifndef RAYTRACING_ONB_H
#define RAYTRACING_ONB_H

#include "src/Random.h"

class onb
{
public:
    onb()  = default;
    ~onb() = default;

    inline glm::vec3 operator[](int i) const { return axis[i]; }

    glm::vec3 u() const { return axis[0]; }
    glm::vec3 v() const { return axis[1]; }
    glm::vec3 w() const { return axis[2]; }

    glm::vec3 local(double a, double b, double c) const {
        return mult(a, u()) + mult(b, v()) + mult(c, w());
    }
    glm::vec3 local(const glm::vec3& a) {
        return mult(a.x, u()) + mult(a.y, v()) + mult(a.z, w());
    }

    void buildFromW(const glm::vec3& n);

private:
    glm::vec3 mult(double a, glm::vec3 v) const {
        return {v.x * a, v.y * a, v.z * a};
    }

public:
    glm::vec3 axis[3];

};


void onb::buildFromW(const glm::vec3& n)
{
    axis[2] = glm::normalize(n);
    glm::vec3 a = (fabs(w().x) > 0.9) ? glm::vec3(0,1,0) : glm::vec3(1,0,0);
    axis[1] = glm::normalize(glm::cross(w(), a));
    axis[0] = glm::cross(w(), v());
}


#endif //RAYTRACING_ONB_H
