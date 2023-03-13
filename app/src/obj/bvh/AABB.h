#ifndef RAYTRACING_AABB_H
#define RAYTRACING_AABB_H

#include "../Hit.h"
#include "../../Ray.h"

class AABB
{
public:
    AABB() = default;
    virtual ~AABB() = default;

    AABB(const glm::vec3& a, const glm::vec3& b) : minimum(a), maximum(b) {}

    glm::vec3 min() const { return minimum; }
    glm::vec3 max() const { return maximum; }

    bool intersect(const Ray &ray, tHit& intersector) const;

public:
    glm::vec3 minimum;
    glm::vec3 maximum;
};

AABB surroundingBox(AABB box_0, AABB box_1);


#endif //RAYTRACING_AABB_H
