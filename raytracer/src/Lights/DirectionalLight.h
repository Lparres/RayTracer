#pragma once

#include "Color.h"
#include "Light.h"

// Implementación concreta de una luz direccional, que hereda de Light
// Define la dirección de la luz (normalizada) y su color
class DirectionalLight : public Light {
public:
    DirectionalLight(glm::vec3 dir, Color c, float intensity = 1.0f);
    ~DirectionalLight() = default;
    LightContribution getLightContribution(const glm::vec3& hitPoint) const override;

private:
    glm::vec3 direction;
};
