#ifndef RAYTRACING_BVH_H
#define RAYTRACING_BVH_H

#include "../Object.h"
#include "../../Scene.h"


class BVH_Node : public Object
{
public:
    BVH_Node();

    BVH_Node(const Scene& scene) : BVH_Node(scene.sceneObjects, 0, scene.sceneObjects.size()) { }

    BVH_Node(const std::vector<std::shared_ptr<Object>>& sceneObjects, size_t start, size_t end);

    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;
    // We need to rebuild the BVH after every transform
    void transform(const glm::mat4& transform) const override { box.transform(transform); }

    void getUI() override {}
    void getUV(const glm::vec3& p, float& u, float& v) const override {}

public:
    std::shared_ptr<Object> left;
    std::shared_ptr<Object> right;
    AABB box;

private:
    std::vector<std::shared_ptr<Object>> m_BVHObjects;

public:
    static bool boxCompare(const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b, int axis);

    static bool boxCompare_x (const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) { return boxCompare(a, b, 0); }
    static bool boxCompare_y (const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) { return boxCompare(a, b, 1); }
    static bool boxCompare_z (const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) { return boxCompare(a, b, 2); }
};

#endif //RAYTRACING_BVH_H
