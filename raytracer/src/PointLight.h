#pragma once

#include "Light.h"

class PointLight : public Light {
public:
    PointLight(glm::vec3 pos, Color c);
    ~PointLight() = default;

    Color shade(Ray r, HitInfo hit) override;

private:
    glm::vec3 position;
};