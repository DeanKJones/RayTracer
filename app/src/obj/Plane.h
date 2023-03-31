#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include "bvh/BVH.h"
#include "Triangle.h"

class Plane : public Object
{
public:
    Plane() : Object() {}
    ~Plane() override = default;

    Plane(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
          bool pVisibility, bool pInReflections, float pSize);

    Plane(float pSize, std::shared_ptr<Material>& pMaterial);

    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;

    void getUV(const glm::vec3& p, float& u, float& v) const override {}
    void getUI() override;

public:
    float m_Size;
    bool hasBackfaceCulling = true;

private:
    std::vector<std::shared_ptr<Object>> m_Tris;
    std::shared_ptr<BVH_Node> m_BVH;
};


#endif //RAYTRACING_PLANE_H
