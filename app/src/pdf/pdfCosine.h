#ifndef RAYTRACING_PDFCOSINE_H
#define RAYTRACING_PDFCOSINE_H

#include "pdf.h"
#include "ONB.h"

class pdfCosine : public pdf
{
public:
    pdfCosine() : pdf() {}
    ~pdfCosine() override = default;

    pdfCosine(const glm::vec3& w) { uvw.buildFromW(w); }

    virtual double value(const glm::vec3& direction) const override
    {
        float cosine = glm::dot(glm::normalize(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine / M_PI;
    }

    virtual glm::vec3 generate() const override
    {
        return uvw.local(Walnut::Random::CosineDirection());
    }


public:
    onb uvw;
};

#endif //RAYTRACING_PDFCOSINE_H
