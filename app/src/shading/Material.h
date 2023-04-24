#pragma once

#include "src/Random.h"
#include "Texture.h"
#include "../Ray.h"

struct Payload;

class Material 
{
public:
    Material() = default;
    virtual ~Material() = default;

    Material(glm::vec3 pAlbedo) : albedo(std::make_shared<SolidColor>(pAlbedo)) {};
    Material(std::shared_ptr<Texture> pTexture) : albedo(pTexture) {};

    virtual glm::vec3 emittedLight(float u, float v, const glm::vec3& p) const {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    virtual bool scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered, float& pdf
        ) const = 0;

    virtual float scatterPDF(const Ray& ray, const Payload& payload, Ray& scattered) const { return 0; }

    glm::vec3 reflect(const glm::vec3 &incident, const glm::vec3 &normal) const;

public:
    std::shared_ptr<Texture> albedo;
};
