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

bool Dielectric::scatter(
    const Ray &ray, const Payload &payload, glm::vec3 &colorAttenuation, Ray &scattered) const
{
    // Maybe try passing albedo color here to see the types of results
    colorAttenuation = albedo;
    float refraction_ratio = payload.frontFace ? (1.0f / indexOfRefraction) : indexOfRefraction;

    glm::vec3 unitDirection = glm::normalize(ray.Direction);

    double cos_theta = glm::min(glm::dot(-unitDirection, payload.worldNormal), 1.0f);
    double sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
    glm::vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > Core::Random::Float()) {
        direction = reflect(unitDirection, payload.worldNormal);
    } else {
        direction = refract(unitDirection, payload.worldNormal, refraction_ratio);
    }

    scattered.Origin = payload.worldPosition;
    scattered.Direction = direction;

    return true;
}

glm::vec3 Dielectric::refract(const glm::vec3 &uv, const glm::vec3 &normal, float etaiOverEtat) const
{
    float cosTheta = fmin(glm::dot(-uv, normal), 1.0f);

    glm::vec3 outPerpendicular = etaiOverEtat * (uv + cosTheta * normal);
    float perpLengthSquared = glm::sqrt(outPerpendicular.length());
    // Forced to cast to float to avoid fabs storing values as double
    glm::vec3 outParallel = -glm::sqrt((float)fabs(1.0 - perpLengthSquared)) * normal;

    return outPerpendicular + outParallel;
}

double Dielectric::reflectance(double cosine, double ref_idx)
{
    // Schlicks approximation for reflectance
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}