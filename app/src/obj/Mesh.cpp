#include "Mesh.h"

Mesh::Mesh(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial, bool pVisibility,
           bool pInReflections)
            : Object(pName, pPosition, pMaterial, pVisibility, pInReflections)
{
}


bool Mesh::intersect(const Ray &ray, tHit &intersector, Payload &payload) const
{
    return false;
}


bool Mesh::boundingBox(AABB &outputBox) const
{
    return false;
}


void Mesh::getUV(const glm::vec3 &p, float &u, float &v) const
{
    // Nothing yet
}


void Mesh::getUI()
{
    // nothing yet
}


bool Mesh::loadOBJ(const std::string &filename)
{
    // Unsigned int read
    auto getUint = [](const char*& ptr)
    {
        size_t val = 0;
        while (*ptr == ' ')
            ptr++;
        if (*ptr == '/')
            ptr++;
        while (*ptr && *ptr != ' ' && *ptr != '/')
            val = val * 10 + *ptr++ - '0';
        return val;
    };
}