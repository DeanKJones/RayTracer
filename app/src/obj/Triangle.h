#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H

#include "glm/glm.hpp"
#include <iostream>

#include "Hit.h"
#include "Object.h"

class Ray;

class Triangle : public Object
{
public:
    Triangle() : Object() {};
    ~Triangle() override = default;

    Triangle(const glm::vec3& pA,   const glm::vec3& pB,   const glm::vec3& pC);

    Triangle(const glm::vec3& pA,   const glm::vec3& pB,   const glm::vec3& pC,
             const glm::vec3& pA_n, const glm::vec3& pB_n, const glm::vec3& pC_n);

    Triangle(const glm::vec3& pA,   const glm::vec3& pB,   const glm::vec3& pC,
             const glm::vec3& pA_n, const glm::vec3& pB_n, const glm::vec3& pC_n,
             const glm::vec2& pA_t, const glm::vec2& pB_t, const glm::vec2& pC_t);

    bool intersect(const Ray &ray, tHit &intersector, Payload &payload) const override;
    bool boundingBox(AABB& outputBox) const override;

    void getUV(const glm::vec3& p, float& u, float& v) const override {};
    void getUI() override;

private:
    bool hasBackfaceCulling = false;

public:
    // Vertex Positions
    glm::vec3 m_A;
    glm::vec3 m_B;
    glm::vec3 m_C;

    // Normals
    glm::vec3 m_An;
    glm::vec3 m_Bn;
    glm::vec3 m_Cn;

    // Texture
    glm::vec2 m_At;
    glm::vec2 m_Bt;
    glm::vec2 m_Ct;

    // Calculate tangent and bi-tangent once
    glm::vec3 m_Tangent;
    glm::vec3 m_BiTangent;
};


#endif //RAYTRACING_TRIANGLE_H
