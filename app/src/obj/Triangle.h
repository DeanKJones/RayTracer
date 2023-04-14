#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H

#include "glm/glm.hpp"
#include <iostream>

#include "Hit.h"
#include "Object.h"

class Ray;
class Mesh;

class Triangle : public Object
{
public:
    Triangle() : Object() {};
    ~Triangle() override = default;

    Triangle(const std::shared_ptr<Mesh> pMesh, const std::array<uint32_t, 3>& indices, std::shared_ptr<Material>& pMaterial, bool pBackface)
            : Object(), mesh(pMesh), indices(indices), hasBackfaceCulling(pBackface) { material_ptr = pMaterial; }

    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;

    void getUV(const glm::vec3& p, float& u, float& v) const override {};
    void getUI() override;

public:
    bool hasBackfaceCulling = false;

private:
    const std::shared_ptr<Mesh> mesh;
    std::array<uint32_t, 3> indices;
};


#endif //RAYTRACING_TRIANGLE_H
