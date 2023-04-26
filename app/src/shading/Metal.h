#ifndef RAYTRACING_METAL_H
#define RAYTRACING_METAL_H

#include "Material.h"

class Metal : public Material
{
public:
    Metal() : Material() {}
    ~Metal() override = default;

    Metal(const glm::vec3 &pColor, float pRoughness) :
            Material(pColor),
            roughness(pRoughness < 1 ? pRoughness : 1) {}

    virtual bool scatter(
            const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered, float& pdf
    ) const override;

public:
    float roughness;
};


#endif //RAYTRACING_METAL_H
