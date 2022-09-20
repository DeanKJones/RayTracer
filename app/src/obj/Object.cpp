#include "Object.h"

////// UI VARIABLES //////
glm::vec3 Sphere::sphereCenter = {0.0f, 0.0f, 0.0f};
float Sphere::radius = 0.5f;
glm::vec3 Sphere::color = {1.0f, 1.0f, 1.0f};

Sphere::Sphere(glm::vec3 pPos, float pRadius, glm::vec3 pColor)
{
    sphereCenter = pPos;
    radius = pRadius;
    color = pColor;
}


bool Sphere::intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const
{
    // (bx^2 + bx^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // Where: 
    // a = Ray Origin
    // b = Ray Direction
    // r = Radius
    // t = Hit Distance

    float t;

    glm::vec3 diff = origin - sphereCenter;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot((diff), rayDirection);  // rayOrigin - spherePosition allows the sphere to move
    float c = glm::dot(diff, diff) - radius * radius;

    if(!solveQuadratic(a, b, c, t)) 
        return false;

    tNear = t;

    return true;
}


void Sphere::getSurfaceData(const glm::vec3 &position, glm::vec3 &normal) const
{
    normal = glm::normalize(position - sphereCenter); 
}


bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, float &t) const
{
    /*  Quadratic Formula
    *         ___________
    *   -b +-/ b^2 - 4ac
    *           2a
    */  

   float t0, t1;

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return false;

    else if (discriminant == 0) { 
        t0 = t1 = - 0.5 * b / a; 
    } 
    else { 
        float quadratic = (b > 0) ? 
            (-0.5 * (b + glm::sqrt(discriminant))) : 
            (-0.5 * (b - glm::sqrt(discriminant))); 
        t0 = quadratic / a; 
        t1 = c / quadratic; 
    } 
    t = t0;

    return true;
}