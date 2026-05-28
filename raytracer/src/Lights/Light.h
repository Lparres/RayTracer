#pragma once

#include <limits>

#include "Ray.h"
#include "Color.h"
#include "HitInfo.h"
#include "glm/geometric.hpp"

// Interfaz abstracta para cualquier tipo de luz en la escena
// Define el método computeLighting() que calcula la contribución de la luz en un punto de intersección
class Light {
public:

    // Rayo que se lanza desde el punto de intersección hacia la luz para comprobar si hay sombras
    struct ShadowRay {
        Ray ray;
        float maxDistance;
    };

    virtual ~Light() = default;

    // Devuelve la contribución de la luz en el punto de intersección proporcionado.
    // virtual Color computeLighting(const Ray &incoming, const HitInfo &hit) const = 0;

    // Genera el rayo de sombra y la distancia máxima de comprobación para esta luz.
    virtual ShadowRay getShadowRay(const glm::vec3& hitPoint) const = 0;

    Color getColor() const { return color; }
    glm::vec3 getPosOrDir() const { return posOrDir; }
    bool castsShadows() const { return castShadows; }

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
