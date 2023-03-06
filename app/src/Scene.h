#pragma once

#include "obj/Object.h"
#include "obj/Sphere.h"
#include "obj/Line.h"

#include <vector>

class Ray;
class Renderer;

class Scene : public Object
{
public:
    Scene() = default;
    ~Scene() override = default;

    Scene(std::shared_ptr<Object> object) { AddToScene(object); }

    void CreateDefaultScene();
    void CreateNewSphere();

    void RayPathToLine(Renderer &pRender);

    void AddToScene(std::shared_ptr<Object> object);
    void RemoveItem(int objectIndex);

    void Clear() { sceneObjects.clear(); }
    void ClearRays();

    bool intersect(const Ray &ray, tHit &intersector, int &objectIndex) const;

    Object GetItem(int objectIndex);

    // Virtual Declarations
    bool intersect(const Ray &ray, tHit &intersector) const override {}
    bool intersectBB(AABB& outputBox) const override {}
    void getUI() override {}
    void getUV(const glm::vec3& p, float& u, float& v) override {}

public:
    std::vector<std::shared_ptr<Object>> sceneObjects;
    mutable std::vector<Ray> rayToLine;

    int rayToLineCount = 0;
};