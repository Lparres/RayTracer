#pragma once

#include "Ray.h"
#include "Color.h"
#include "HitInfo.h"
#include "glm/geometric.hpp"

// Interfaz abstracta para cualquier tipo de luz en la escena
// Define el método shade() que calcula la contribución de la luz en un punto de intersección
class Light {
public:
    ~Light() = default;

    virtual Color shade(Ray r, HitInfo hit) = 0;

    bool castsShadows() { return castShadows; }

    virtual glm::vec3 getShadowDir(glm::vec3 pos) { return glm::vec3();}

protected:
    Color color;

    bool castShadows;

    Light(Color c, bool castShadows) : color(c), castShadows(castShadows) {}
};