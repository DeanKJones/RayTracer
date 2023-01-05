
#include "Line.h"
#include "imgui.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"

Line::Line(std::string pName, glm::vec3 pPosition,
           std::shared_ptr<Material> pMaterial,
           bool pVisibility, glm::vec3 pDestination, float pThickness)
                : Object(pName, pPosition, pMaterial, pVisibility),
                  destination(pDestination),
                  thickness(pThickness) { }


bool Line::intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &tNear) const
{
    /*  Solving for the intersection between two lines
     *    This is solved by finding points along either line.
     *
     *      p1 = r1 + t1*e1                             (1)
     *      p2 = r2 + t2*e2
     *
     *    t1 and t2 are scalar values along two lines respectively
     *    r1 and r2 are points along the two lines
     *    e1 and e2 are non-unit direction vectors of the lines
     *
     *    n = e1 x e2                                   (2)
     *      If the cross product is equal to 0 then the lines are parallel
     *
     *    d = ((n . p1) / ||n||) - ((n . p2) / ||n||)   (3)
     *      = (n . (p1 - p2)) / ||n||
     *      = (n . (r1 − r2+ t1*e1 − t2*e2)) / ||n||
     *
     *      Since the dot product of n.e1 and n.e2 = 0
     *      We can simplify further:
     *
     *    d = |(n . (r1 - r2))| / ||n||
     *
     *    To find the location of p1 and p2:
     *
     *    t1 = ((e2 x n) . (r2 - r1)) / n . n          (4)
     *    t2 = ((e1 x n) . (r2 - r1)) / n . n
     *
     *    Substitute t back into the first equation -> (1) to get
     *    the position on the line that is closest.
     *
     * */

    glm::vec3 lineDirection = GetLineDirection();
    glm::vec3 vec3null(0,0,0);

    glm::vec3 n = glm::cross(rayDirection, lineDirection);
    if (n == vec3null) {
        return false;   // The two lines are parallel
    }

    float dist = (abs(glm::dot(n, (rayOrigin - rayDirection)))) / glm::length(n);

    if (dist <= thickness) {

        // TODO: Error in the math here, returning 0 distance sometimes which becomes a negative 0

        float t1 = glm::dot(glm::cross(lineDirection, n), (position - rayOrigin)) / glm::dot(n, n);
        float t2 = glm::dot(glm::cross(rayDirection, n), (position - rayOrigin)) / glm::dot(n, n);

        glm::vec3 RayPoint  = rayOrigin + (t1 * rayDirection);
        glm::vec3 LinePoint = position + (t2 * lineDirection);

        tNear = t1;

        if (dist == glm::length(RayPoint - LinePoint)) {
            return true;
        }
    }
}

void Line::getUI()
{
    char *objName = this->objectName.data();
    ImGui::Text("%s is selected", objName);

    ImGui::Separator();

    ImGui::ColorEdit3(": Color", glm::value_ptr(this->material_ptr->albedo));
    ImGui::DragFloat3(": Position", glm::value_ptr(this->position), 0.1f);

    ImGui::DragFloat3(": Line origin", glm::value_ptr(this->position), 0.1f);
    ImGui::DragFloat3(": Line destination", glm::value_ptr(this->destination), 0.1f);

    ImGui::Checkbox(": Visibility", &this->isVisible);
    ImGui::DragFloat(": Line thickness", &this->thickness, 0.001f);
}