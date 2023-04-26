#ifndef RAYTRACING_HIT_H
#define RAYTRACING_HIT_H

struct tHit
{
    tHit() = default;
    tHit(float pNear, float pFar) : t_near(pNear), t_far(pFar) {}

    float t_near;
    float t_far;
};

#endif //RAYTRACING_HIT_H
