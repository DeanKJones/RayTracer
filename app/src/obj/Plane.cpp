#include "Plane.h"

#include "imgui.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"


Plane::Plane(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial, bool pVisibility,
             bool pInReflections, float pSize)
      : Object(pName, pPosition, pMaterial, pVisibility, pInReflections), m_Size(pSize)
{
}


Plane::Plane(float pSize, std::shared_ptr<Material>& pMaterial)
{
    m_Size = pSize;
    // Make center by halving size
    float halfSize = m_Size / 2;

    // Create corners
    glm::vec3 p0 = {-halfSize,   0.0f, -halfSize};
    glm::vec3 p1 = {halfSize,    0.0f, -halfSize};
    glm::vec3 p2 = {halfSize,    0.0f, halfSize};
    glm::vec3 p3 = {-halfSize,   0.0f, halfSize};

    // Create Triangles
    std::shared_ptr<Triangle> half1 = std::make_shared<Triangle>(p0, p3, p1);
    std::shared_ptr<Triangle> half2 = std::make_shared<Triangle>(p1, p3, p2);

    half1->material_ptr = pMaterial;
    half1->hasBackfaceCulling = hasBackfaceCulling;
    half2->material_ptr = pMaterial;
    half2->hasBackfaceCulling = hasBackfaceCulling;

    // Push into triangle pointer
    m_Tris.push_back(half1);
    m_Tris.push_back(half2);

    m_BVH = std::make_shared<BVH_Node>(m_Tris, 0, m_Tris.size());
}


bool Plane::intersect(const Ray &ray, tHit &intersector, Payload &payload) const
{
    if(m_BVH->intersect(ray, intersector, payload)) {
        return true;
    }
    return false;
}


bool Plane::boundingBox(AABB &outputBox) const
{
    outputBox = m_BVH->box;
    return true;
}


void Plane::getUI()
{
    char *objName = this->objectName.data();
    ImGui::Text("%s is selected", objName);

    // Need to add back material editing in the UI

    ImGui::Separator();

    ImGui::DragFloat3(": Triangle position", glm::value_ptr(this->position), 0.1f);
    ImGui::Checkbox(": Triangle visibility", &this->isVisible);
    ImGui::Checkbox(": Reflection Bounces", &this->inReflections);

    ImGui::Checkbox(": Backface Culling", &this->hasBackfaceCulling);

}
