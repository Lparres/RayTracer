#pragma once

#include <memory>
#include <cmath>
#include "Color.h"

// Coordenadas UV en el espacio local de la superficie.
// Normalizadas a [0,1]x[0,1]
struct UV {
    float u;
    float v;
};

// Interfaz base para todo tipo de texturas.
// Las texturas son value-like: clonar produce una copia independiente.
class Texture {
public:
    virtual ~Texture() = default;

    // Devuelve el color de la textura en las coordenadas UV dadas.
    virtual Color sample(UV uv) const = 0;

    // Produce una copia profunda de la textura.
    virtual std::shared_ptr<Texture> clone() const = 0;

protected:
    // Transforma una coordenada UV cualquiera a su equivalente dentro del rango [0,1].
    static float wrap(float x) {
        float w = std::fmod(x, 1.0f);
        return w < 0.0f ? w + 1.0f : w;
    }

    static float clamp(float x) {
        return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);
    }
};

