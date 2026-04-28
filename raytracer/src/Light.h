#pragma once

#include "Ray.h"
#include "Color.h"
#include "HitInfo.h"

// Interfaz abstracta para cualquier tipo de luz en la escena
// Define el método shade() que calcula la contribución de la luz en un punto de intersección
class Light {
public:
    ~Light() = default;

    virtual Color shade(Ray r, HitInfo hit) = 0;

protected:
    Color color;

    Light(Color c) : color(c) {}
};