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

private:
    glm::vec3 albedo;
};