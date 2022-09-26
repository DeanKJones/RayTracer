#include "Object.h"

////// GLOBAL VARIABLES /////
int Object::nextID = 0;
std::vector<glm::vec3> Sphere::color {};
std::vector<glm::vec3> Sphere::sphereCenter {};
std::vector<float> Sphere::radius {};

Sphere::Sphere(glm::vec3 pPos, float pRadius, glm::vec3 pColor)
{
    sphereCenter.push_back(pPos);
    radius.push_back(pRadius);
    color.push_back(pColor);
    ID = nextID++;
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
    glm::vec3 sphereCenter = getSpherePosition();
    float radius = getSphereRadius();

    glm::vec3 diff = origin - sphereCenter;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot((diff), rayDirection);  // rayOrigin - spherePosition allows the sphere to move
    float c = glm::dot(diff, diff) - radius * radius;

    if(!solveQuadratic(a, b, c, t)) 
        return false;
    else {
        tNear = t;
        return true;
    }
}


void Sphere::getSurfaceData(const glm::vec3 &hitPosition, glm::vec3 &normal, glm::vec3 &surfaceColor) const
{
    glm::vec3 objectCenter = getSpherePosition();
    normal = glm::normalize(hitPosition - objectCenter); 
    surfaceColor = getSphereColor();
}


bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, float &t) const
{
    /*  Quadratic Formula
    *         ___________
    *   -b +-/ b^2 - 4ac   =  t
    *           2a
    */  

    float discriminant = (b * b) - (4.0f * a * c);
    if (discriminant < 0.0f)
        return false;

    else 
        t = ((-b - glm::sqrt(discriminant)) / (2 * a));

    ////// Scratch Pixel Technique //////
    // 
    // else if (discriminant == 0) { 
    //     t0 = t1 = - 0.5 * b / a; 
    // } 
    // else { 
    //     float quadratic = (b > 0) ? 
    //         (-0.5 * (b + glm::sqrt(discriminant))) : 
    //         (-0.5 * (b - glm::sqrt(discriminant))); 
    //     t0 = quadratic / a; 
    //     t1 = c / quadratic; 
    // } 

    return true;
}