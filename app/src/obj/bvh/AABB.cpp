
#include "AABB.h"

#include "algorithm"

bool AABB::intersect(const Ray &ray, tHit& intersector) const
{
#define Kensler 1
#if Kensler
    for (int a = 0; a < 3; a++)
    {
        float invD = 1.0f / ray.Direction[a];
        float t0 = (min()[a] - ray.Origin[a]) * invD;
        float t1 = (max()[a] - ray.Origin[a]) * invD;

        if (invD < 0.0f)
            std::swap(t0, t1);

        // Use Temp near and far values
        // Avoids right leaf from getting cut off
        float t_near_temp;
        float t_far_temp;

        t_near_temp = t0 > intersector.t_near ? t0 : intersector.t_near;
        t_far_temp  = t1 < intersector.t_far  ? t1 : intersector.t_far;

        if (t_far_temp <= t_near_temp) {
            return false;
        }
    }
    return true;
#else
    for (int a = 0; a < 3; a++) {
        auto t0 = fmin((min()[a] - ray.Origin[a]) / ray.Direction[a],
                             (max()[a] - ray.Origin[a]) / ray.Direction[a]);
        auto t1 = fmax((min()[a] - ray.Origin[a]) / ray.Direction[a],
                             (max()[a] - ray.Origin[a]) / ray.Direction[a]);
        intersector.t_near = fmax(t0, intersector.t_near);
        intersector.t_far = fmin(t1, intersector.t_far);
        if (intersector.t_far <= intersector.t_near)
            return false;
    }
    return true;
#endif Kensler
}

AABB surroundingBox(AABB box_0, AABB box_1)
{
    glm::vec3 small = {fmin(box_0.min().x, box_1.min().x),
                       fmin(box_0.min().y, box_1.min().y),
                       fmin(box_0.min().z, box_1.min().z) };

    glm::vec3 big   = {fmax(box_0.max().x, box_1.max().x),
                       fmax(box_0.max().y, box_1.max().y),
                       fmax(box_0.max().z, box_1.max().z) };

    return AABB{small, big};
}
