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

    struct LightContribution {
        ShadowRay shadowRay;
        glm::vec3 wi;
        Color Li;
    };

    virtual ~Light() = default;

    // Devuelve la contribución de la luz en el punto de intersección proporcionado.
    // virtual Color computeLighting(const Ray &incoming, const HitInfo &hit) const = 0;

    virtual LightContribution getLightContribution(const glm::vec3& hitPoint) const = 0;
    bool castsShadows() const { return castShadows; }

protected:
    Light(Color c, bool castShadows) :
        color(c),
        castShadows(castShadows)
    {}

    Color color;
    bool castShadows;
};
