#ifndef RAYTRACING_LINE_H
#define RAYTRACING_LINE_H

#include "Object.h"

class Line : public Object
{
public:
    Line() : Object() {};
    ~Line() {};

    Line(const glm::vec3 &pOrigin, const glm::vec3 &pDestination, const glm::vec3 &pColor, float pThickness);
    bool intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &tNear) const override;

private:
    glm::vec3 GetLineDirection() const { return destination - origin; }

private:
    glm::vec3 origin;
    glm::vec3 destination;
    glm::vec3 color;
    float thickness;
};

#endif //RAYTRACING_LINE_H
