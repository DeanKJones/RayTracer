#include "Triangle.h"
#include "Mesh.h"

#include "imgui.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"

bool Triangle::intersect(const Ray &ray, tHit &intersector, Payload &payload) const
{
    const glm::vec3& v0 = mesh->getPosition(indices[0]);
    const glm::vec3& v1 = mesh->getPosition(indices[1]);
    const glm::vec3& v2 = mesh->getPosition(indices[2]);

    const glm::vec3 triNormal = glm::cross(v1 - v0, v2 - v0);

    glm::vec3 v0v1  = v1 - v0;
    glm::vec3 v0v2  = v2 - v0;
    glm::vec3 p_vec = glm::cross(ray.Direction, v0v2);

    float det = glm::dot(v0v1, p_vec);

    // Backface Culling
    if (hasBackfaceCulling) {
        if (det < 1e-8) {
            return false;
        }
    }
    if (fabs(det) < 1e-8) {
        return false;
    }

    float invD = 1.0f / det;

    glm::vec3 t_vec = ray.Origin - v0;
    float u = (glm::dot(t_vec, p_vec)) * invD;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    glm::vec3 q_vec = glm::cross(t_vec, v0v1);
    float v = (glm::dot(ray.Direction, q_vec)) * invD;
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    intersector.t_far = glm::dot(v0v2, q_vec) * invD;
    if (intersector.t_far < 0.0f) {
        return false;
    }

    // Set Payload
    payload.hitDistance = intersector.t_far;
    payload.hitPosition = ray.at(payload.hitDistance);

    glm::vec3 normal = glm::normalize(((triNormal * u) + (triNormal * v) + (triNormal * (1 - u - v))) / 3.0f);
    payload.setFaceNormal(ray.Direction, normal);

    payload.materialPtr = getMaterialPtr();
    payload.objectType = "Mesh";
    payload.v = v;
    payload.u = u;

    return true;
}


bool Triangle::boundingBox(AABB &outputBox) const
{
    glm::vec3 m_A = mesh->getPosition(indices[0]);
    glm::vec3 m_B = mesh->getPosition(indices[1]);
    glm::vec3 m_C = mesh->getPosition(indices[2]);

    float minX = std::min(m_A.x, std::min(m_B.x, m_C.x));
    float minY = std::min(m_A.y, std::min(m_B.y, m_C.y));
    float minZ = std::min(m_A.z, std::min(m_B.z, m_C.z));

    float maxX = std::max(m_A.x, std::max(m_B.x, m_C.x));
    float maxY = std::max(m_A.y, std::max(m_B.y, m_C.y));
    float maxZ = std::max(m_A.z, std::max(m_B.z, m_C.z));

    float buffer = 0.00001f;

    glm::vec3 min = {minX - buffer, minY - buffer, minZ - buffer};
    glm::vec3 max = {maxX + buffer, maxY + buffer, maxZ + buffer};

    outputBox = AABB(min, max);
    return true;
}



void Triangle::getUI()
{
    char *objName = this->objectName.data();
    ImGui::Text("%s is selected", objName);

    ImGui::Separator();

    //ImGui::ColorEdit3(": Sphere color", glm::value_ptr(this->material_ptr->albedo));
    std::string typeidName = typeid(*(this->material_ptr)).name();

    if (typeidName.find("Metal") != std::string::npos) {
        auto sphereRoughness = &(reinterpret_cast<Metal *>(this->material_ptr.get())->roughness);
        ImGui::DragFloat(": Roughness", sphereRoughness, 0.05f, 0.0f, 1.0f);
    }
    if (typeidName.find("Dielectric") != std::string::npos) {
        auto sphereIOR = &(reinterpret_cast<Dielectric *>(this->material_ptr.get())->indexOfRefraction);
        ImGui::DragFloat(": IOR", sphereIOR, 0.05f, 0.0f, 1.0f);
    }

    ImGui::DragFloat3(": Triangle position", glm::value_ptr(this->position), 0.1f);
    ImGui::Checkbox(": Triangle visibility", &this->isVisible);
    ImGui::Checkbox(": Reflection Bounces", &this->inReflections);
    ImGui::Checkbox(": Backface Culling", &this->hasBackfaceCulling);

    //ImGui::DragFloat3(": Vertex 0 Position", glm::value_ptr(mesh->getPosition(indices[0])), 0.1f);
    //ImGui::DragFloat3(": Vertex 1 Position", glm::value_ptr(mesh->getPosition(indices[1])), 0.1f);
    //ImGui::DragFloat3(": Vertex 2 Position", glm::value_ptr(mesh->getPosition(indices[2])), 0.1f);
}