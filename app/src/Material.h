#pragma once

#include "../../core/src/Random.h"
#include "glm/glm.hpp"
#include "Ray.h"

struct Payload;

class Material 
{
public:
    virtual bool scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
        ) const = 0;

};

class Lambertian : public Material 
{
public:
    Lambertian(const glm::vec3 &pColor) : albedo(pColor) {}
    
    virtual bool scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
        ) const override;
    
    bool nearZero(glm::vec3& nearingZero) const;

private:
    glm::vec3 albedo;

public:
    bool GetLambertModel() const { return lambertHemi; }
    static bool lambertHemi;
};

class Metal : public Material
{
public:
    Metal(const glm::vec3 &pColor) : albedo(pColor) {}

    virtual bool scatter(
        const Ray& ray, const Payload& payload, glm::vec3& colorAttenuation, Ray& scattered
        ) const override;

    glm::vec3 reflect(const glm::vec3 &vector, const glm::vec3 &normal) const;

private:
    glm::vec3 albedo;
};