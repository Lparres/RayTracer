#pragma once

#include "Color.h"
#include "Light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight(glm::vec3 dir, Color c);
    ~DirectionalLight() = default;

    Color shade(Ray r, HitInfo hit) override;

private:
    glm::vec3 direction;
};