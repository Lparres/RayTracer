#pragma once

#include "Ray.h"
#include "Color.h"
#include "HitInfo.h"
#include "glm/geometric.hpp"

// Interfaz abstracta para cualquier tipo de luz en la escena
// Define el método shade() que calcula la contribución de la luz en un punto de intersección
class Light {
public:
    virtual ~Light() = default;

    virtual Color shade(Ray r, HitInfo hit) = 0;

    bool castsShadows() const { return castShadows; }

    virtual glm::vec3 getShadowDir(glm::vec3 pos) const { return glm::vec3();}

    glm::vec3 getPosOrDir() const { return posOrDir; }

protected:
    Light(Color c, glm::vec3 posOrDir, bool castShadows) :
        color(c),
        posOrDir(posOrDir),
        castShadows(castShadows)
    {}

    Color color;
    glm::vec3 posOrDir;
    bool castShadows;
};
