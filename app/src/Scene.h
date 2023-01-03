#pragma once

#include "obj/Object.h"
#include "obj/Sphere.h"
#include "obj/Line.h"

#include <vector>

struct Scene
{
    std::vector<Sphere> objects;
    std::vector<Line> lines;
};
