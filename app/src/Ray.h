#pragma once

#include <glm/glm.hpp>

struct Ray
{
    Ray() = default;
    Ray(glm::vec3 pOrigin, glm::vec3 pDirection) : Origin(pOrigin), Direction(pDirection) {}

	glm::vec3 Origin;
	glm::vec3 Direction;

    mutable float HitDistance;

    glm::vec3 at(float rootDist) const {
        return Origin + (rootDist * Direction);
    }

    bool isFirstBounce;
}; 