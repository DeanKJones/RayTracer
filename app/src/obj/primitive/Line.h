#ifndef RAYTRACING_LINE_H
#define RAYTRACING_LINE_H

#include "../Object.h"

class Line : public Object
{
public:
    Line() : Object() {};
    ~Line() override = default;

    Line(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
         bool pVisibility, bool pInReflections, glm::vec3 pDestination, float pThickness);

    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;

    void getUI() override;
    void getUV(const glm::vec3& p, float& u, float& v) const override {}

private:
    glm::vec3 GetLineDirection() const { return glm::normalize(destination - position); }

public:
    glm::vec3 destination;
    float thickness;
};

#endif //RAYTRACING_LINE_H
