#pragma once

#include "Color.h"
#include "Light.h"

// Implementación concreta de una luz direccional, que hereda de Light
// Define la dirección de la luz (normalizada) y su color
class DirectionalLight : public Light {
public:
    DirectionalLight(glm::vec3 dir, Color c);
    ~DirectionalLight() = default;

    Color shade(Ray r, HitInfo hit) override;

private:
    glm::vec3 direction;
};