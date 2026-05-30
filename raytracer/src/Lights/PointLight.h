#pragma once

#include "Light.h"

// Implementación concreta de una luz puntual, que hereda de Light
// Define la posición de la luz y su color
class PointLight : public Light {
public:
    PointLight(glm::vec3 pos, Color c, float intensity = 1.0f);
    ~PointLight() = default;

    LightContribution getLightContribution(const glm::vec3& hitPoint) const override;

private:
    glm::vec3 position;
};
