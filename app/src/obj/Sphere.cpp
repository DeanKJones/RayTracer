
#include "Sphere.h"

#include "imgui.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"


Sphere::Sphere(std::string pName, glm::vec3 pPosition, std::shared_ptr<Material> pMaterial,
               bool pVisibility, bool pInReflections, float pRadius) :
        Object(pName, pPosition, pMaterial, pVisibility, pInReflections), radius(pRadius) { }

bool Sphere::intersect(const Ray &ray, tHit &quadratic, Payload &payload) const
{
    // Preparing quadratic
    /* (bx^2 + bx^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    * Where:
    * a = Ray Origin
    * b = Ray Direction
    * r = Radius
    * t = Hit Distance
    */

    // The sphere center is subtracted from the ray origin to allow for the sphere to move and get hit by rays
    glm::vec3 diff = ray.Origin - position;
    float a = glm::dot(ray.Direction, ray.Direction);
    float b = 2.0f * glm::dot((diff), ray.Direction);
    float c = glm::dot(diff, diff) - radius * radius;

    if(solveQuadratic(a, b, c, quadratic)) {
        if (quadratic.t_far <= 0){
            return false;
        }
        else {
            payload.hitDistance = quadratic.t_far;

            payload.hitPosition = ray.at(payload.hitDistance);
            glm::vec3 outwardNormal = (payload.hitPosition - position) / radius;
            payload.setFaceNormal(ray.Direction, outwardNormal);

            glm::vec3 origin = ray.Origin - position;
            payload.hitPosition = origin + ray.Direction * payload.hitDistance;
            payload.materialPtr = getMaterialPtr();

            getUV(outwardNormal, payload.u, payload.v);

            // Add sphere position back
            payload.hitPosition += position;
            payload.objectType   = "Sphere";

            ray.HitDistance = payload.hitDistance;
            return true;
        }
    }
    return false;
}

bool Sphere::boundingBox(AABB& outputBox) const
{
    auto radiusBuffer = radius + 0.000001f;
    outputBox = AABB((position - glm::vec3(radiusBuffer, radiusBuffer, radiusBuffer)),
                     (position + glm::vec3(radiusBuffer, radiusBuffer, radiusBuffer)));
    return true;
}


bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, tHit &quadratic) const {
    // Quadratic
    /*  Quadratic Formula
    *         ___________
    *   -b +-/ b^2 - 4ac   =  t
    *           2a
    */

    float discriminant = (b * b) - (4.0f * a * c);
    if (discriminant < 0.0f) {
        return false;
    }

    float root = ((-b - glm::sqrt(discriminant)) / (2 * a));
    if (root < quadratic.t_near || quadratic.t_far < root) {
        root = ((-b + glm::sqrt(discriminant)) / (2 * a));
        if (root < quadratic.t_near || quadratic.t_far < root) {
            return false;
        }
    }
    quadratic.t_far = root;
    return true;
}

void Sphere::getUV(const glm::vec3& p, float& u, float& v) const
{
    /*
    *  p: a given point on the sphere of radius one, centered at the origin.
    *  u: returned value [0,1] of angle around the Y axis from X=-1.
    *  v: returned value [0,1] of angle from Y=-1 to Y=+1.
    *      < 1 0 0 > yields <0.50 0.50>      < -1  0  0 > yields <0.00 0.50>
    *      < 0 1 0 > yields <0.50 1.00>      <  0 -1  0 > yields <0.50 0.00>
    *      < 0 0 1 > yields <0.25 0.50>      <  0  0 -1 > yields <0.75 0.50>
    */

    float theta = acos(-p.y);
    float phi = atan2(-p.z, p.x) + glm::pi<float>();

    u = phi / (2 * glm::pi<float>());
    v = theta / glm::pi<float>();
}

void Sphere::getUI()
{
    char *objName = this->objectName.data();
    ImGui::Text("%s is selected", objName);

    ImGui::Separator();

    //ImGui::ColorEdit3(": Sphere color", glm::value_ptr(this->material_ptr->albedo));
    std::string typeidName = typeid(*(this->material_ptr)).name();

    if (typeidName.find("Metal") != std::string::npos) {
        auto sphereRoughness = &(reinterpret_cast<Metal *>(this->material_ptr.get())->roughness);
        ImGui::DragFloat(": Roughness", sphereRoughness, 0.05f, 0.0f, 1.0f);
    }
    if (typeidName.find("Dielectric") != std::string::npos) {
        auto sphereIOR = &(reinterpret_cast<Dielectric *>(this->material_ptr.get())->indexOfRefraction);
        ImGui::DragFloat(": IOR", sphereIOR, 0.05f, 0.0f, 1.0f);
    }

    ImGui::DragFloat3(": Sphere position", glm::value_ptr(this->position), 0.1f);
    ImGui::Checkbox(": Sphere visibility", &this->isVisible);
    ImGui::Checkbox(": Reflection Bounces", &this->inReflections);
    ImGui::DragFloat(": Sphere radius", &this->radius, 0.1f);
}
