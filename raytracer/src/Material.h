#pragma once

#include "Color.h"

// Material representa las propiedades de la superficie de un objeto
// Usada por las luces calcular la respuesta de iluminación
class Material {
public:
    Material(const Color& albedo, float specular = 30.f, float reflectance = 0.f) : albedo(albedo), specular(specular), reflectance(reflectance) {}
    virtual ~Material() = default;

    Color get_albedo() const { return albedo; }
    float get_specular() const { return specular; }
    float get_reflectance() const { return reflectance; }

private:
    Color albedo;
    float specular;
    float reflectance;
};