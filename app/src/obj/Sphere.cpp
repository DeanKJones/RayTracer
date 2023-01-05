
#include "Sphere.h"
#include "../Payload.h"

#include "imgui.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"


Sphere::Sphere(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial, bool pVisibility, float pRadius) :
        Object(pName, pPosition, pMaterial, pVisibility), radius(pRadius) {}

bool Sphere::intersect(const glm::vec3 &origin, const glm::vec3 &rayDirection, float &tNear) const
{
    // Preparing quadratic
    /* (bx^2 + bx^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    * Where:
    * a = Ray Origin
    * b = Ray Direction
    * r = Radius
    * t = Hit Distance
    */

    float t;
    glm::vec3 sphereCenter = getObjectPosition();
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


bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, float &t) const
{
    // Quadratic
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

void Sphere::getUI()
{
    char *objName = this->objectName.data();
    ImGui::Text("%s is selected", objName);

    ImGui::Separator();

    ImGui::ColorEdit3(": Color", glm::value_ptr(this->material_ptr->albedo));
    std::string typeidName = typeid(*(this->material_ptr)).name();

    if (typeidName.find("Metal") != std::string::npos) {
        auto sphereRoughness = &(reinterpret_cast<Metal *>(this->material_ptr.get())->roughness);
        ImGui::DragFloat(": Roughness", sphereRoughness, 0.05f, 0.0f, 1.0f);
    }
    if (typeidName.find("Dielectric") != std::string::npos) {
        auto sphereIOR = &(reinterpret_cast<Dielectric *>(this->material_ptr.get())->indexOfRefraction);
        ImGui::DragFloat(": IOR", sphereIOR, 0.05f, 0.0f, 1.0f);
    }

    ImGui::DragFloat3(": Position", glm::value_ptr(this->position), 0.1f);
    ImGui::Checkbox(": Visibility", &this->isVisible);
    ImGui::DragFloat(": Size", &this->radius, 0.1f);
}
