#pragma once

#include "../../core/src/Random.h"
#include "glm/glm.hpp"
#include "Ray.h"

struct Payload;

class Material 
{
public:
    Material() = default;
    virtual ~Material() = default;

    Material(glm::vec3 pAlbedo) : albedo(pAlbedo) {};

    virtual bool scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
        ) const = 0;

public:
    glm::vec3 albedo;
};

class Lambertian : public Material 
{
public:
    Lambertian() = default;
    ~Lambertian() override = default;

    Lambertian(const glm::vec3 &pColor) : Material(pColor) {}
    
    virtual bool scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
        ) const override;
    
    bool nearZero(glm::vec3& nearingZero) const;

public:
    bool GetLambertModel() const { return lambertHemi; }
    static bool lambertHemi;
};

class Metal : public Material
{
public:
    Metal() = default;
    ~Metal() override = default;

    Metal(const glm::vec3 &pColor, float pRoughness) : 
            Material(pColor),
            roughness(pRoughness < 1 ? pRoughness : 1) {}

    virtual bool scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
        ) const override;

    glm::vec3 reflect(const glm::vec3 &vector, const glm::vec3 &normal) const;

public:
    // Using float since glm doesn't support double
    float roughness;
};