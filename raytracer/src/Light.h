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

    glm::vec3 getPosOrDir() { return posOrDir; }

protected:
    Color color;
    
    glm::vec3 posOrDir;
    
    bool castShadows;

    Light(Color c, glm::vec3 posOrDir, bool castShadows) : color(c), posOrDir(posOrDir), castShadows(castShadows) {}
};