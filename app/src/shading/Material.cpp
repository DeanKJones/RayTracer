#include "Material.h"
#include "../Payload.h"

// MATERIAL FUNCTIONS
glm::vec3 Material::reflect(const glm::vec3 &incident, const glm::vec3 &normal) const
{
    return incident - 2 * glm::dot(incident, normal) * normal;
}
