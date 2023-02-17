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

    glm::vec3 reflect(const glm::vec3 &incident, const glm::vec3 &normal) const;

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

public:
    // Using float since glm doesn't support double
    float roughness;
};


class Dielectric : public Material
{
public:
    Dielectric() = default;
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
