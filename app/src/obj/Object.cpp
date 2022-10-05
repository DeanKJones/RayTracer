#include "Object.h"

Sphere::Sphere(glm::vec3 pPos, float pRadius, glm::vec3 pColor, std::shared_ptr<Material> material)
{
    position = pPos;
    radius = pRadius;
    albedo = pColor;
    material_ptr = material;
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

    // The sphere center is subtracted from the ray origin to allow for the sphere to move and get hit by rays
    glm::vec3 diff = origin - sphereCenter; 
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot((diff), rayDirection);
    float c = glm::dot(diff, diff) - radius * radius;

    if(solveQuadratic(a, b, c, t)) {
        if (t <= 0){
            return false;
        }
        else {
            tNear = t;
            return true;
        }
    }
    return false;
}


// std::shared_ptr<Material> Sphere::getSurfaceData() const
// { 
//     getMaterialPtr();
// }


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
        // Unused
        //tFar = ((-b + glm::sqrt(discriminant)) / (2 * a));

    return true;
}