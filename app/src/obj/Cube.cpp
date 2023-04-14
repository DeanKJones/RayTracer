
#include "Cube.h"
#include "imgui.h"
#include "../shading/Lambertian.h"
#include "glm/gtc/type_ptr.hpp"

Cube::Cube(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
            bool pVisibility, bool pInReflections, float pSize)
            : Mesh(pName, pPosition, pMaterial, pVisibility, pInReflections), m_Size(pSize)
{
}

Cube::Cube(float pSize, glm::vec3 pPosition, std::shared_ptr<Material> &pMaterial)
{
    position = pPosition;
    m_Size = pSize;
    // Make center by halving size
    float halfSize = m_Size / 2;

    // Create Corners
    vertexPositions.push_back({position.x + halfSize, position.y + halfSize, position.z - halfSize});
    vertexPositions.push_back({position.x + halfSize, position.y + halfSize, position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize, position.y + halfSize, position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize, position.y + halfSize, position.z - halfSize});
    vertexPositions.push_back({position.x + halfSize, position.y - halfSize, position.z - halfSize});
    vertexPositions.push_back({position.x + halfSize, position.y - halfSize, position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize, position.y - halfSize, position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize, position.y - halfSize, position.z - halfSize});

    // Create Triangles
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{1, 2, 6},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{1, 6, 5},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 1, 5},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 5, 4},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{3, 0, 4},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{3, 4, 7},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{2, 3, 7},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{2, 7, 6},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 3, 2},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 2, 1},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{5, 6, 7},
                                                pMaterial, hasBackfaceCulling));

    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{5, 7, 4},
                                                pMaterial, hasBackfaceCulling));

    m_BVH = std::make_shared<BVH_Node>(m_Tris, 0, m_Tris.size());
}


void Cube::CornellBox()
{
    position = {0.0f, 0.0f, 0.0f};
    m_Size = 2;
    // Make center by halving size
    float halfSize = m_Size / 2;

    // Create Corners
    vertexPositions.push_back({position.x + halfSize, position.y + m_Size, position.z - halfSize});
    vertexPositions.push_back({position.x + halfSize, position.y + m_Size, position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize, position.y + m_Size, position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize, position.y + m_Size, position.z - halfSize});
    vertexPositions.push_back({position.x + halfSize,    position.y,          position.z - halfSize});
    vertexPositions.push_back({position.x + halfSize,    position.y,          position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize,    position.y,          position.z + halfSize});
    vertexPositions.push_back({position.x - halfSize,    position.y,          position.z - halfSize});

    std::shared_ptr<Material> Red = std::make_shared<Lambertian>(glm::vec3(0.9f, 0.0f, 0.0f));
    std::shared_ptr<Material> Blue = std::make_shared<Lambertian>(glm::vec3(0.0f, 0.0f, 0.9f));
    std::shared_ptr<Material> Green = std::make_shared<Lambertian>(glm::vec3(0.0f, 0.9f, 0.0f));
    std::shared_ptr<Material> White = std::make_shared<Lambertian>(glm::vec3(0.9f, 0.9f, 0.9f));

    // Create Triangles
    // Front
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{1, 6, 2},
                                                Blue, hasBackfaceCulling));
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{1, 5, 6},
                                                Blue, hasBackfaceCulling));

    // Right
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 5, 1},
                                                Green, hasBackfaceCulling));
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 4, 5},
                                                Green, hasBackfaceCulling));

    // Back
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{3, 4, 0},
                                                White, hasBackfaceCulling));
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{3, 7, 4},
                                                White, hasBackfaceCulling));

    // Left
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{2, 7, 3},
                                                Red, hasBackfaceCulling));
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{2, 6, 7},
                                                Red, hasBackfaceCulling));

    // Top
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 2, 3},
                                                White, hasBackfaceCulling));
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{0, 1, 2},
                                                White, hasBackfaceCulling));

    // Bottom
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{5, 7, 6},
                                                White, hasBackfaceCulling));
    m_Tris.push_back(std::make_shared<Triangle>(static_cast<const std::shared_ptr<Mesh>>(this),
                                                std::array<uint32_t , 3>{5, 4, 7},
                                                White, hasBackfaceCulling));

    m_BVH = std::make_shared<BVH_Node>(m_Tris, 0, m_Tris.size());
}


bool Cube::intersect(const Ray &ray, tHit &intersector, Payload &payload) const
{
    if(m_BVH->intersect(ray, intersector, payload)) {
        return true;
    }
    return false;
}


bool Cube::boundingBox(AABB &outputBox) const
{
    outputBox = m_BVH->box;
    return true;
}


void Cube::getUI()
{
    char *objName = this->objectName.data();
    ImGui::Text("%s is selected", objName);

    // Need to add back material editing in the UI

    ImGui::Separator();

    ImGui::DragFloat3(": Cube position", glm::value_ptr(this->position), 0.1f);
    ImGui::Checkbox(": Cube visibility", &this->isVisible);
    ImGui::Checkbox(": Reflection Bounces", &this->inReflections);
}