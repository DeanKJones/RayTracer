#ifndef RAYTRACING_EMISSIVE_H
#define RAYTRACING_EMISSIVE_H

#include "Material.h"

class Emissive : public Material
{
public:
    Emissive() : Material() {}
    ~Emissive() override = default;

    Emissive(std::shared_ptr<Texture> pTexture) : m_Emit(pTexture) {}
    Emissive(glm::vec3 pEmissiveColor) : m_Emit(std::make_shared<SolidColor>(pEmissiveColor)) {}

    virtual glm::vec3 emittedLight(float u, float v, const glm::vec3& p) const override {
        return m_Emit->value(u, v, p);
    }

    inline virtual bool scatter(
            const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered, float& pdf
    ) const override { return false; }

public:
    std::shared_ptr<Texture> m_Emit;
};


#endif //RAYTRACING_EMISSIVE_H
