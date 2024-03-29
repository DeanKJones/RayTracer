#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include "bvh/BVH.h"
#include "Mesh.h"

class Plane : public Mesh
{
public:
    Plane() : Mesh() {}
    ~Plane() override = default;

    Plane(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
          bool pVisibility, bool pInReflections, float pSize);

    Plane(float pSize, glm::vec3 pPosition, std::shared_ptr<Material>& pMaterial);
    Plane(float pSize, glm::vec3 pPosition, std::shared_ptr<Material>& pMaterial, bool flipped, bool backface);

    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;

    double pdfValue(const glm::vec3& origin, const glm::vec3& v) const override;
    glm::vec3 random(const glm::vec3& origin) const override;

    void getUV(const glm::vec3& p, float& u, float& v) const override {}
    void getUI() override;

public:
    float m_Size;
    bool hasBackfaceCulling = true;
};


#endif //RAYTRACING_PLANE_H
