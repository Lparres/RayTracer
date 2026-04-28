#pragma once

#include <memory>

#include "glm/vec3.hpp"
#include "Material.h"

// HitInfo representa el resultado de una intersección entre un rayo y un objeto
class HitInfo {
public:
    glm::vec3 p;
    glm::vec3 normal;
    float t;
    std::shared_ptr<Material> material;

    // Coordenadas locales del plano (u, v) para texturizado
    float u;
    float v;
};
