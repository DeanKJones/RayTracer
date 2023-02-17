#pragma once

#include "obj/Object.h"
#include "obj/Sphere.h"
#include "obj/Line.h"

#include <vector>

class Ray;
class Renderer;

class Scene
{
public:
    Scene()  = default;
    ~Scene() = default;

    void CreateDefaultScene();
    void CreateNewSphere();

    void RayPathToLine(Renderer &pRender);

    void RemoveItem(int objectIndex);
    void ClearRays();

    Object GetItem(int objectIndex);

public:
    std::vector<Object *> sceneObjects;
    mutable std::vector<Ray> rayToLine;

private:
    std::vector<Sphere> spheres;
    std::vector<Line> lines;

    int rayToLineCount = 0;
};