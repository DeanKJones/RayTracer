
#include "Dielectric.h"
#include "../Payload.h"
#include <random>

bool Dielectric::scatter(
        const Ray &ray, const Payload &payload, glm::vec3 &colorAttenuation, Ray &scattered) const
{
    colorAttenuation = albedo->value(payload.u, payload.v, payload.hitPosition);
    float eta = payload.frontFace ? (1.0f / indexOfRefraction) : indexOfRefraction;

    glm::vec3 incidentRay = glm::normalize(ray.Direction);

    double cos_theta = fmin(glm::dot(-incidentRay, payload.worldNormal), 1.0f);
    double sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);
    bool cannot_refract = eta * sin_theta > 1.0f;

    glm::vec3 direction;

    if (cannot_refract || reflectance(cos_theta, eta) > Walnut::Random::Float()) {
        direction = reflect(incidentRay, payload.worldNormal);
    } else {
        direction = refract(incidentRay, payload.worldNormal, eta);
    }

    // Move the origin forward inside the glass
    scattered.Origin = payload.hitPosition + (direction * 0.00005f);
    scattered.Direction = direction;

    return true;
}

glm::vec3 Dielectric::refract(const glm::vec3& incident, const glm::vec3& n, float etai_over_etat) const
{
    float cos_theta = fmin(glm::dot(-incident, n), 1.0f);

    glm::vec3 r_out_perp =  etai_over_etat * (incident + cos_theta * n);
    float r_o_p_lengthSq = (r_out_perp.x * r_out_perp.x) +
                           (r_out_perp.y * r_out_perp.y) +
                           (r_out_perp.z * r_out_perp.z);
    glm::vec3 r_out_parallel = -glm::sqrt(fabs(1.0f - r_o_p_lengthSq)) * n;

    return r_out_perp + r_out_parallel;
}

void Dielectric::refract_2(const glm::vec3 &incident, const glm::vec3 &normal, float &ior, glm::vec3 &refracted) const {
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

glm::vec3 Dielectric::refract_3(glm::vec3 i, glm::vec3 n, float eta, glm::vec3 outRay) const
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