#pragma once

#include "Color.h"

// Material representa las propiedades de la superficie de un objeto
// Usada por las luces calcular la respuesta de iluminación
class Material {
public:
    Material(const Color& albedo) : albedo(albedo) {}
    virtual ~Material() = default;

    Color get_albedo() const { return albedo; }

private:
    Color albedo;
};