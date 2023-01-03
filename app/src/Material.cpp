#include "Material.h"
#include "Payload.h"
#include <random>

// Globals
bool Lambertian::lambertHemi = false;

// MATERIAL FUNCTIONS
glm::vec3 Material::reflect(const glm::vec3 &incident, const glm::vec3 &normal) const
{
    return incident - 2 * glm::dot(incident, normal) * normal;
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

    scattered.Origin = payload.hitPosition + (payload.worldNormal * 0.00001f);
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

    scattered.Origin = payload.hitPosition + (payload.worldNormal * 0.00001f);
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

    // eta = srcIOR / destIOR
    //float eta = payload.frontFace ? (1.0f / indexOfRefraction) : indexOfRefraction;
    float eta = 1.0f;

    glm::vec3 incidentRay = glm::normalize(ray.Direction);
    //glm::vec3 incidentRay = glm::abs(ray.Direction);

    double cos_theta = fmin(glm::dot(incidentRay, payload.worldNormal), 1.0f);
    double sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);
    bool cannot_refract = eta * sin_theta > 1.0f;

    glm::vec3 direction;

    if (cannot_refract || reflectance(cos_theta, eta) > random()) {
    //if (cannot_refract) {
        direction = reflect(incidentRay, payload.worldNormal);
    } else {
        //refract(unitDirection, payload.worldNormal, refraction_ratio, direction);
        //direction = refract2(unitDirection, payload.worldNormal, refraction_ratio);
        refract3(incidentRay, payload.worldNormal, eta, direction);
    }

    scattered.Origin = payload.hitPosition;
    scattered.Direction = direction;

    return true;
}

void Dielectric::refract(const glm::vec3 &incident, const glm::vec3 &normal, float &ior, glm::vec3 &refracted) const {
    float eta = ior;
    float eta2 = eta * eta;

    glm::vec3 nTi = normal * incident;
    glm::vec3 nTi2 = nTi * nTi;

    glm::vec3 root = glm::sqrt(1.0f - eta2 * (1.0f - nTi2));
    glm::vec3 sinThetaT = eta * root;

    glm::vec3 cosThetaT = glm::sqrt(1.0f - sinThetaT * sinThetaT);
    //refracted = eta * incident + (eta * root - cosThetaT) * normal;

    refracted = (root * normal) + (eta * (incident - (nTi * normal)));

    /*refracted = glm::sqrt(1 - (eta * eta) * (1 - (float) glm::pow(glm::dot(incident, normal), 2))) * normal +
                    eta * (incident - glm::dot(incident, normal) * normal);*/
}

glm::vec3 Dielectric::refract2(const glm::vec3& uv, const glm::vec3& n, float etai_over_etat) const
{
    float cos_theta = fmin(glm::dot(-uv, n), 1.0f);

    glm::vec3 r_out_perp =  etai_over_etat * (uv + cos_theta * n);
    glm::vec3 r_out_parallel = -glm::sqrt(glm::abs(1.0f - glm::length(r_out_perp))) * n;

    return r_out_perp + r_out_parallel;
}

glm::vec3 Dielectric::refract3(glm::vec3 i, glm::vec3 n, float eta, glm::vec3 outRay) const
{
    glm::vec3 normal = glm::normalize(n);

    // Tests
    glm::vec3 iPerp = glm::dot(i, normal) * normal;
    glm::vec3 iPar = i - iPerp;

    //eta = 2.0f - eta;
    const float cosi = -glm::dot(n, i);
    const float sinT2 = eta * eta * (1.0f - cosi * cosi);
    if (sinT2 > 1.0f){
        return outRay = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    const float cosT = glm::sqrt(1.0f - sinT2);
    outRay = eta * i + (eta + cosi - cosT) * n;


    glm::vec3 iEtaN = (i * eta) - n;
    //outRay = iEtaN * (-cosi + eta * cosi);
    //outRay = (i * eta - n * (-cosi + eta * cosi));

    return outRay;

    //sin^2Tt = eta^2 * sin^2Ti
}

double Dielectric::reflectance(double cosine, double ref_idx)
{
    // Schlicks approximation for reflectance
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}