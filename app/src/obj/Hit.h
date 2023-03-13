#ifndef RAYTRACING_HIT_H
#define RAYTRACING_HIT_H

#include <iostream>
#include "math.h"

class tHit
{
public:
    tHit() = default;
    virtual ~tHit() = default;

    tHit(float pNear, float pFar) : t_near(pNear), t_far(pFar) {  }
    tHit(const tHit& a, const tHit& b) :
        t_near(fmin(a.t_near, b.t_near)),
        t_far(fmax(a.t_far, b.t_far)) {  }

    bool contains(float x) const {
        return t_near <= x && x <= t_far;
    }
    float clamp(float x) {
        if (x < t_near) return t_near;
        if (x > t_far) return t_far;
        return x;
    }

//    tHit operator+(const tHit& ival, float displacement) {
//        return tHit(ival.t_near + displacement, ival.t_far + displacement);
//    }
//
//    tHit operator+(float displacement, const tHit& ival) {
//        return ival + displacement;
//    }

public:
    float t_near;
    float t_far;
};

#endif //RAYTRACING_HIT_H
