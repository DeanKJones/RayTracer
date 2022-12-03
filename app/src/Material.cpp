#include "Material.h"
#include "Payload.h"

// Globals
bool Lambertian::lambertHemi = false;

// MATERIAL FUNCTIONS
glm::vec3 Material::reflect(const glm::vec3 &vector, const glm::vec3 &normal) const
{
    return vector - 2 * glm::dot(vector, normal) * normal;
}


// LAMBERTIAN MATERIALS

bool Lambertian::scatter(
    const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered) const 
{

    glm::vec3 scatterDirection;
    bool lambertMode = GetLambertModel();

    if (lambertMode){
        scatterDirection = payload.worldNormal + Core::Random::InUnitHemi(payload.worldNormal);
    } else {
        scatterDirection = payload.worldNormal + Core::Random::InUnitSphere();
    }

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
    glm::vec3 reflected = reflect(ray.Direction, payload.worldNormal);

    reflected = reflected + (roughness * Core::Random::InUnitSphere());

    scattered.Origin = payload.worldPosition + (payload.worldNormal * 0.00001f);
    scattered.Direction = reflected;
    colorAttenuation = albedo;

    return (glm::dot(scattered.Direction, payload.worldNormal) > 0);
}

// DIELECTRIC MATERIALS
/*
 * TODO: Solve sometime
 * First render after the scene's init will render glass incorrectly
 * rendering the scene again (with or without changes) will then correct the glass material
 */

bool Dielectric::scatter(
    const Ray &ray, const Payload &payload, glm::vec3 &colorAttenuation, Ray &scattered) const
{
    colorAttenuation = albedo;
    float refraction_ratio = payload.frontFace(ray.Direction, payload.worldNormal)
                            ? (1.0f / indexOfRefraction) : indexOfRefraction;

    glm::vec3 unitDirection = glm::normalize(ray.Direction);

    double cos_theta = glm::min(glm::dot(-unitDirection, payload.worldNormal), 1.0f);
    double sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);
    bool cannot_refract = refraction_ratio * sin_theta > 1.0f;

    glm::vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > Core::Random::Float()) {
        direction = reflect(unitDirection, payload.worldNormal);
    } else {
        refract(unitDirection, payload.worldNormal, refraction_ratio, direction);
    }

    scattered.Origin = payload.worldPosition;
    scattered.Direction = direction;

    return true;
}

void Dielectric::refract(const glm::vec3 &incident, const glm::vec3 &normal, float &ior, glm::vec3 &refracted) const
{
    float eta = ior;
    float cosThetaI = glm::dot(incident, normal);
    float sinThetaI = glm::sqrt(1.0f - cosThetaI * cosThetaI);
    float sinThetaT = eta * sinThetaI;

    if (sinThetaT >= 1.0f) {
        refracted = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else {
        float cosThetaT = glm::sqrt(1.0f - sinThetaT * sinThetaT);
        refracted = eta * incident + (eta * cosThetaI - cosThetaT) * normal;
    }
}


double Dielectric::reflectance(double cosine, double ref_idx)
{
    // Schlicks approximation for reflectance
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}