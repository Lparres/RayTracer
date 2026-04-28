#pragma once

#include <memory>

#include "glm/vec3.hpp"
#include "Material.h"

// HitInfo representa el resultado de una intersección entre un rayo y un objeto
class HitInfo {
public:
    glm::vec3 p;
    glm::vec3 normal;
    double t;
    std::shared_ptr<Material> material;

    // uv
    float u;
    float v;
};