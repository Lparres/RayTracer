#pragma once

#include "Color.h"
#include "Light.h"

// Implementación concreta de una luz direccional, que hereda de Light
// Define la dirección de la luz (normalizada) y su color
class DirectionalLight : public Light {
public:
    DirectionalLight(glm::vec3 dir, Color c);
    ~DirectionalLight() = default;
    Color computeLighting(const Ray& incoming, const HitInfo& hit) const override;
    ShadowRay getShadowRay(const glm::vec3& hitPoint) const override;
};
