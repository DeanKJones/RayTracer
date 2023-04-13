#ifndef RAYTRACING_MESH_H
#define RAYTRACING_MESH_H

#include "bvh/BVH.h"
#include "Triangle.h"

#include <vector>

class Mesh : public Object
{
public:
    Mesh() : Object() {};
    ~Mesh() override = default;

    Mesh(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
         bool pVisibility, bool pInReflections);

    // Intersects with a bvh node built over all the triangles
    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    // calls the m_BVH upper box
    bool boundingBox(AABB& outputBox) const override;

    void getUV(const glm::vec3& p, float& u, float& v) const override;
    void getUI() override;

public:
    bool loadOBJ(const std::string& filename);

    inline const glm::vec3& getPosition(uint32_t index) const { return vertexPositions[index]; }

protected:
    std::vector<glm::vec3> vertexPositions;
    std::vector<glm::vec3> vertexNormals;

    std::vector<std::shared_ptr<Object>> m_Tris;
    std::shared_ptr<BVH_Node> m_BVH;

};


#endif //RAYTRACING_MESH_H