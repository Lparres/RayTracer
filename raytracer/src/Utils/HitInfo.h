#pragma once

#include <memory>
#include "glm/vec3.hpp"
#include "Material.h"
#include "Texture.h"   // Para UV

// Resultado de una intersección entre un rayo y un objeto
class HitInfo {
public:
    glm::vec3 p{};
    glm::vec3 normal{};
    glm::vec3 tangent{};    // para mapas de normales
    glm::vec3 bitangent{};  // para mapas de normales
    float     t = 0.0f;
    UV        uv{};
    std::shared_ptr<Material> material;
};
