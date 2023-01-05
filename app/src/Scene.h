#pragma once

#include "obj/Object.h"
#include "obj/Sphere.h"
#include "obj/Line.h"

#include <vector>

class Scene
{
public:
    Scene()  = default;
    ~Scene() = default;

    void CreateDefaultScene();
    void CreateNewSphere();

    void RemoveItem(int objectIndex);
    Object GetItem(int objectIndex);

public:
    std::vector<Object *> sceneObjects;
private:
    std::vector<Sphere> spheres;
    std::vector<Line> lines;
};