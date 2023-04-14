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
            const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
    ) const override;

    bool nearZero(glm::vec3& nearingZero) const;

public:
    bool GetLambertModel() const { return lambertHemi; }
    static bool lambertHemi;
};


#endif //RAYTRACING_LAMBERTIAN_H
