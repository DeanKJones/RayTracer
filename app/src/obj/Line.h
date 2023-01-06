#ifndef RAYTRACING_LINE_H
#define RAYTRACING_LINE_H

#include "Object.h"

class Line : public Object
{
public:
    Line() : Object() {};
    ~Line() {};

    Line(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
         bool pVisibility, glm::vec3 pDestination, float pThickness);

    bool intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &tNear) const override;
    void getUI() override;

private:
    glm::vec3 GetLineDirection() const { return destination - position; }

public:
    glm::vec3 destination;
    float thickness;
};

#endif //RAYTRACING_LINE_H
