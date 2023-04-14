#ifndef RAYTRACING_DIELECTRIC_H
#define RAYTRACING_DIELECTRIC_H

#include "Material.h"

class Dielectric : public Material
{
public:
    Dielectric() : Material() {}
    ~Dielectric() override = default;

    Dielectric(glm::vec3 pColor, float pIR) : Material(pColor), indexOfRefraction(pIR) {}
    virtual bool scatter(
            const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
    ) const override;

    glm::vec3 refract(const glm::vec3& incident, const glm::vec3& n, float etai_over_etat) const;
    void refract_2(const glm::vec3 &incident, const glm::vec3 &normal, float &ior, glm::vec3 &refracted) const;
    glm::vec3 refract_3(glm::vec3 i, glm::vec3 n, float eta, glm::vec3 outRay) const;

    static double reflectance(double cosine, double ref_idx);

public:
    float indexOfRefraction;
};


#endif //RAYTRACING_DIELECTRIC_H
