#include "Lambertian.h"
#include "../Payload.h"
#include "../utils/ONB.h"

// LAMBERTIAN MATERIALS

bool Lambertian::scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered) const
{

    glm::vec3 scatterDirection;
    bool lambertMode = GetLambertModel();

    if (lambertMode){
        scatterDirection = payload.worldNormal + Walnut::Random::InUnitHemi(payload.worldNormal);
    } else {
        scatterDirection = payload.worldNormal + Walnut::Random::InUnitSphere();
    }

    // Need to catch degenerate ray scatters
    if(nearZero(scatterDirection)){
        scatterDirection = payload.worldNormal;
    }

    onb uvw;
    uvw.buildFromW(payload.worldNormal);
    scatterDirection = uvw.local(Walnut::Random::RandomCosineDirection());

    scattered.Origin = payload.hitPosition + (payload.worldNormal * 0.00001f);
    scattered.Direction = scatterDirection;
    pdf = glm::dot(uvw.w(), scatterDirection) / M_PI;

    colorAttenuation = albedo->value(payload.u, payload.v, payload.hitPosition);
    return true;
}

bool Lambertian::nearZero(glm::vec3& nearingZero) const
{
    const auto s = 1e-8;
    bool near = (fabs(nearingZero[0] < s) && fabs(nearingZero[1]) < s && fabs(nearingZero[2]) < s);
    return near;
}
