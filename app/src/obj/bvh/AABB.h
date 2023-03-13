#ifndef RAYTRACING_AABB_H
#define RAYTRACING_AABB_H

#include "../Hit.h"
#include "../../Ray.h"

class AABB
{
public:
    AABB() = default;
    virtual ~AABB() = default;

    AABB(const tHit& ix, const tHit& iy, const tHit& iz) : x(ix), y(iy), z(iz) { }
    AABB(const glm::vec3& a, const glm::vec3& b);
    AABB(const AABB& box_0, const AABB& box_1);

    const tHit& axis(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool intersect(const Ray &ray, tHit& intersector) const;

public:
    tHit x, y, z;
};

#endif //RAYTRACING_AABB_H
