#include "Material.h"
#include "Payload.h"

bool Lambertian::scatter(
    const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered) const 
{
    auto scatterDirection = payload.worldNormal + Core::Random::InUnitSphere();

    // Need to catch degenerate ray scatters
    scattered.Origin = payload.worldPosition + (payload.worldNormal * 0.0001f);
    scattered.Direction = scatterDirection - payload.worldPosition;

    colorAttenuation = albedo;
    return true;
}