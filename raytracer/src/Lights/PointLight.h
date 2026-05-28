#pragma once

#include "Light.h"

// Implementación concreta de una luz puntual, que hereda de Light
// Define la posición de la luz y su color
class PointLight : public Light {
public:
    PointLight(glm::vec3 pos, Color c);
    ~PointLight() = default;

    ShadowRay getShadowRay(const glm::vec3& hitPoint) const override;
};
