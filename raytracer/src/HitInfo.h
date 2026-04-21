#pragma once

#include <memory>

#include "glm/vec3.hpp"
#include "Material.h"

class HitInfo {
public:
    glm::vec3 p;
    glm::vec3 normal;
    double t;
    std::shared_ptr<Material> material;
};