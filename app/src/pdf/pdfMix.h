#ifndef RAYTRACING_PDFMIX_H
#define RAYTRACING_PDFMIX_H

#include "pdf.h"

#include "../core/src/Random.h"
#include <iostream>

class pdfMix : public pdf
{
public:
    pdfMix(std::shared_ptr<pdf> pdf_1, std::shared_ptr<pdf> pdf_2)
    {
        p[0] = pdf_1;
        p[1] = pdf_2;
    }

    double value(const glm::vec3& direction) const override
    {
        return (0.5f * p[0]->value(direction)) + (0.5f * p[1]->value(direction));
    }

    glm::vec3 generate() const override
    {
        if (Walnut::Random::Float() < 0.5) {
            return p[0]->generate();
        } else {
            return p[1]->generate();
        }
    }

public:
    std::shared_ptr<pdf> p[2];
};


#endif //RAYTRACING_PDFMIX_H
