#ifndef RAYTRACING_CUBE_H
#define RAYTRACING_CUBE_H

#include "Mesh.h"

class Cube : public Mesh
{
public:
    Cube() : Mesh() {}
    ~Cube() override = default;

    Cube(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
            bool pVisibility, bool pInReflections, float pSize);

    Cube(float pSize, glm::vec3 pPosition, std::shared_ptr<Material>& pMaterial);

public:
    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;

    void getUV(const glm::vec3& p, float& u, float& v) const override {}
    void getUI() override;

public:
    float m_Size;
    bool hasBackfaceCulling = true;
};

#endif //RAYTRACING_CUBE_H
