#ifndef RAYTRACING_LAMBERTIAN_H
#define RAYTRACING_LAMBERTIAN_H

#include "Material.h"

class Lambertian : public Material
{
public:
    Lambertian() : Material() {}
    ~Lambertian() override = default;

    Lambertian(const glm::vec3 &pColor) : Material(pColor) {}
    Lambertian(std::shared_ptr<Texture> pTexture) : Material(pTexture) {}

    virtual bool scatter(
            const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered, float& pdf
    ) const override;

    virtual float scatterPDF(const Ray& ray, const Payload& payload, Ray& scattered) const override;

    bool nearZero(glm::vec3& nearingZero) const;
};


#endif //RAYTRACING_LAMBERTIAN_H
