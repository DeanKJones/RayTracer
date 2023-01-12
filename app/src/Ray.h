#pragma once

#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;

    glm::vec3 at(float rootDist) const {
        return Origin + (rootDist * Direction);
    }

    bool isFirstBounce;
}; 