
#include "AABB.h"

#include "algorithm"

AABB::AABB(const glm::vec3& a, const glm::vec3& b)
{
    x = tHit(fmin(a[0],b[0]), fmax(a[0],b[0]));
    y = tHit(fmin(a[1],b[1]), fmax(a[1],b[1]));
    z = tHit(fmin(a[2],b[2]), fmax(a[2],b[2]));
}

AABB::AABB(const AABB &box_0, const AABB &box_1)
{
    x = tHit(box_0.x, box_1.x);
    y = tHit(box_0.y, box_1.y);
    z = tHit(box_0.z, box_1.z);
}

bool AABB::intersect(const Ray &ray, tHit& intersector) const
{
    bool intersected = false;
    for (int a = 0; a < 3; a++)
    {
        float invD = 1.0f / ray.Direction[a];
        float t0 = (axis(a).t_near - ray.Origin[a]) * invD;
        float t1 = (axis(a).t_far - ray.Origin[a]) * invD;

        if (invD < 0.0f)
            std::swap(t0, t1);

        intersector.t_near = t0 > intersector.t_near ? t0 : intersector.t_near;
        intersector.t_far  = t1 < intersector.t_far  ? t1 : intersector.t_far;

        if (intersector.t_far <= intersector.t_near){
            intersected = false;
        } else {
            intersected = true;
        }
    }
    return intersected;
}
