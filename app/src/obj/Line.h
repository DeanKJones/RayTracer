#ifndef RAYTRACING_LINE_H
#define RAYTRACING_LINE_H

#include "Object.h"

class Line : public Object
{
public:
    Line() : Object() {};
    ~Line() {};

    Line(const glm::vec3 &origin, const glm::vec3 &destination, const glm::vec3 &color);
    bool intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const override;

private:
    glm::vec3 pColor;
};

#endif //RAYTRACING_LINE_H
