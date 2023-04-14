#include "Metal.h"
#include "../Payload.h"

// METAL MATERIALS

bool Metal::scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered) const
{
    glm::vec3 rayVector = glm::normalize(ray.Direction);
    glm::vec3 reflected = reflect(ray.Direction, payload.worldNormal);

    reflected = reflected + (roughness * Walnut::Random::InUnitSphere());

    scattered.Origin = payload.hitPosition + (payload.worldNormal * 0.00001f);
    scattered.Direction = reflected;
    colorAttenuation = albedo->value(payload.u, payload.v, payload.hitPosition);

    return (glm::dot(scattered.Direction, payload.worldNormal) > 0);
}