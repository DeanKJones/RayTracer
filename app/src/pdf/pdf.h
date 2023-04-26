#ifndef RAYTRACING_PDF_H
#define RAYTRACING_PDF_H

#include <glm/glm.hpp>

class pdf
{
public:
    pdf() = default;
    virtual ~pdf() {}

    virtual double value(const glm::vec3& direction) const = 0;
    virtual glm::vec3 generate() const = 0;
};

#endif //RAYTRACING_PDF_H
