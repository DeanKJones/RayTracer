#include "Material.h"
#include "Payload.h"


// LAMBERTIAN MATERIALS

bool Lambertian::scatter(
    const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered) const 
{
    auto scatterDirection = payload.worldNormal + Core::Random::InUnitSphere();

    // Need to catch degenerate ray scatters
    if(nearZero(scatterDirection)){
        scatterDirection = payload.worldNormal;
    }

    scattered.Origin = payload.worldPosition + (payload.worldNormal * 0.00001f);
    scattered.Direction = scatterDirection;

    colorAttenuation = albedo;
    return true;
}

bool Lambertian::nearZero(glm::vec3& nearingZero) const 
{
    const auto s = 1e-8;
    bool near = (fabs(nearingZero[0] < s) && fabs(nearingZero[1]) < s && fabs(nearingZero[2]) < s);
    return near;
}


// METAL MATERIALS

bool Metal::scatter(
    const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered) const
{
    glm::vec3 rayVector = glm::normalize(ray.Direction);
    glm::vec3 reflected = reflect(rayVector, payload.worldNormal); 

    scattered.Origin = payload.worldPosition + (payload.worldNormal * 0.00001f);
    scattered.Direction = reflected; 
    colorAttenuation = albedo;

    return (glm::dot(scattered.Direction, payload.worldNormal) > 0);
}

glm::vec3 Metal::reflect(const glm::vec3 &vector, const glm::vec3 &normal) const
{
    return vector - 2 * glm::dot(vector, normal) * normal;
}