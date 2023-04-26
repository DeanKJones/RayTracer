#ifndef RAYTRACING_PDFOBJECT_H
#define RAYTRACING_PDFOBJECT_H

#include "pdf.h"
#include "../obj/Object.h"

class pdfObject : public pdf
{
public:
    pdfObject(const std::shared_ptr<Object>& pObj, const glm::vec3& pOrigin) : objPtr(pObj), origin(pOrigin) {}

     double value(const glm::vec3& pDirection) const override {
        return objPtr->pdfValue(origin, pDirection);
    }

     glm::vec3 generate() const override {
        return objPtr->random(origin);
    }

public:
    std::shared_ptr<Object> objPtr;
    glm::vec3 origin;
};

#endif //RAYTRACING_PDFOBJECT_H
