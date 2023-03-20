#pragma once

#include "obj/Line.h"
#include "obj/Object.h"
#include "obj/Sphere.h"

#include <vector>

class Ray;
class Renderer;
class BVH_Node;

class Scene : public Object
{
public:
    Scene() = default;
    ~Scene() override = default;

    Scene(std::shared_ptr<Object> object) { AddToScene(object); }

    void CreateDefaultScene();
    void CreateNewSphere();
    void CreateRandomSpheres(int numberOfSpheres, float min, float max);

    void RayPathToLine(Renderer &pRender);
    void BuildBvhBox(const AABB &box);
    void TraverseBvhNode(std::shared_ptr<BVH_Node> Node);

    void AddToScene(std::shared_ptr<Object> object);
    void AddToUI(std::shared_ptr<Object> object);
    void RemoveItem(int objectIndex);

    void Clear() { sceneObjects.clear(); }
    void ClearRays();

    // Virtual Declarations
    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;
    void getUI() override {}
    void getUV(const glm::vec3& p, float& u, float& v) const override {}

public:
    std::vector<std::shared_ptr<Object>> sceneObjects;
    std::vector<std::shared_ptr<Object>> ObjectsUI;
    mutable std::vector<Ray> rayToLine;

    int rayToLineCount = 0;
};