
#include "ONB.h"

void onb::buildFromW(const glm::vec3& n)
{
    axis[2] = glm::normalize(n);
    glm::vec3 a = (fabs(w().x) > 0.9) ? glm::vec3(0,1,0) : glm::vec3(1,0,0);
    axis[1] = glm::normalize(glm::cross(w(), a));
    axis[0] = glm::cross(w(), v());
}