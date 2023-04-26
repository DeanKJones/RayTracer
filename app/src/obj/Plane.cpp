#include "Plane.h"

#include "imgui.h"
#include "../Scene.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/norm.hpp"


Plane::Plane(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial, bool pVisibility,
             bool pInReflections, float pSize)
      : Mesh(pName, pPosition, pMaterial, pVisibility, pInReflections), m_Size(pSize)
{
}


Plane::Plane(float pSize, glm::vec3 pPosition, std::shared_ptr<Material>& pMaterial)
{
    position = pPosition;
    m_Size = pSize;
    // Make center by halving size
    float halfSize = m_Size / 2;

    // Create corners
    Vertex vertex_0 {};
    vertex_0.Position = {position.x - halfSize, position.y, position.z - halfSize};
    Vertex vertex_1 {};
    vertex_1.Position = {position.x + halfSize, position.y, position.z - halfSize};
    Vertex vertex_2 {};
    vertex_2.Position = {position.x + halfSize, position.y, position.z + halfSize};
    Vertex vertex_3 {};
    vertex_3.Position = {position.x - halfSize, position.y, position.z + halfSize};

    m_Vertices.push_back(vertex_0);
    m_Vertices.push_back(vertex_1);
    m_Vertices.push_back(vertex_2);
    m_Vertices.push_back(vertex_3);

    // Create Triangles
    std::shared_ptr<Triangle> half1 = std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                                 std::array<uint32_t , 3>{0, 3, 1},
                                                                 pMaterial, hasBackfaceCulling);
    std::shared_ptr<Triangle> half2 = std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                                 std::array<uint32_t , 3>{1, 3, 2},
                                                                 pMaterial, hasBackfaceCulling);

    // Push into triangle pointer
    m_Tris.push_back(half1);
    m_Tris.push_back(half2);

    m_BVH = std::make_shared<BVH_Node>(m_Tris, 0, m_Tris.size());
}


Plane::Plane(float pSize, glm::vec3 pPosition, std::shared_ptr<Material>& pMaterial, bool flipped, bool backface)
{
    position = pPosition;
    m_Size = pSize;
    hasBackfaceCulling = backface;
    // Make center by halving size
    float halfSize = m_Size / 2;

    // Create corners
    Vertex vertex_0;
    vertex_0.Position = {position.x - halfSize, position.y, position.z - halfSize};
    Vertex vertex_1;
    vertex_1.Position = {position.x + halfSize, position.y, position.z - halfSize};
    Vertex vertex_2;
    vertex_2.Position = {position.x + halfSize, position.y, position.z + halfSize};
    Vertex vertex_3;
    vertex_3.Position = {position.x - halfSize, position.y, position.z + halfSize};

    m_Vertices.push_back(vertex_0);
    m_Vertices.push_back(vertex_1);
    m_Vertices.push_back(vertex_2);
    m_Vertices.push_back(vertex_3);

    // Create Triangles
    if (flipped)
    {
        std::shared_ptr<Triangle> half1 = std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                                     std::array<uint32_t, 3>{0, 1, 3},
                                                                     pMaterial, hasBackfaceCulling);
        std::shared_ptr<Triangle> half2 = std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                                     std::array<uint32_t, 3>{1, 2, 3},
                                                                     pMaterial, hasBackfaceCulling);

        // Push into triangle pointer
        m_Tris.push_back(half1);
        m_Tris.push_back(half2);
    }
    else
    {
        std::shared_ptr<Triangle> half1 = std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                                     std::array<uint32_t, 3>{0, 3, 1},
                                                                     pMaterial, hasBackfaceCulling);
        std::shared_ptr<Triangle> half2 = std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                                     std::array<uint32_t, 3>{1, 3, 2},
                                                                     pMaterial, hasBackfaceCulling);

        // Push into triangle pointer
        m_Tris.push_back(half1);
        m_Tris.push_back(half2);
    }

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


double Plane::pdfValue(const glm::vec3& origin, const glm::vec3& v) const
{
    Payload payload;
    tHit intersector(0.0001f, std::numeric_limits<float>::max());
    Ray ray(origin, v);

    if (!this->intersect(ray, intersector, payload)){
        return 0;
    }

    // Something produces under-lit render here
    float area = (m_Size) * (m_Size);
    float distanceSqr = (payload.hitDistance * payload.hitDistance) * glm::dot(v, v);
    float cosine = std::fabs(glm::dot(v, payload.worldNormal) / glm::length(v));

    return distanceSqr / (cosine * area);
}


glm::vec3 Plane::random(const glm::vec3& origin) const
{
    float halfSize = m_Size / 2;
    glm::vec3 randomPoint = {Walnut::Random::Float(position.x - halfSize, position.x + halfSize),
                             position.y,
                             Walnut::Random::Float(position.z - halfSize, position.z + halfSize)};
    return randomPoint - origin;
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
}

