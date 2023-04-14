#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "glm/glm.hpp"
#include <iostream>

class Texture
{
public:
    Texture() = default;
    virtual ~Texture() = default;

    virtual glm::vec3 value(float u, float v, const glm::vec3 p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor() = default;
    ~SolidColor() override = default;

    SolidColor(glm::vec3 pColor) : m_ColorValue(pColor) {}

    SolidColor(double r, double g, double b)
                : SolidColor(glm::vec3(r, g, b)) {}

    glm::vec3 value(float u, float v, const glm::vec3 p) const override
                { return m_ColorValue; }

    glm::vec3 m_ColorValue;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture() = default;
    ~CheckerTexture() override = default;

    CheckerTexture(std::shared_ptr<Texture> pOdd, std::shared_ptr<Texture> pEven)
                : m_Odd(pOdd), m_Even(pEven) {}

    CheckerTexture(glm::vec3 colorEven, glm::vec3 colorOdd)
                : m_Even(std::make_shared<SolidColor>(colorEven)),
                  m_Odd(std::make_shared<SolidColor>(colorOdd))
                  {}

    glm::vec3 value(float u, float v, const glm::vec3 p) const override;

public:
    std::shared_ptr<Texture> m_Odd;
    std::shared_ptr<Texture> m_Even;
};

#endif //RAYTRACING_TEXTURE_H
