#pragma once

#include "Color.h"

// Material representa las propiedades de la superficie de un objeto
// Usada por las luces calcular la respuesta de iluminación
class Material {
public:
    Material(const Color& albedo, float specular = 30.f, float reflectance = 0.f) :
        albedo(albedo),
        specular(specular),
        reflectance(reflectance)
    {}

    virtual ~Material() = default;

    Color getAlbedo() const { return albedo; }
    float getSpecular() const { return specular; }
    float getReflectance() const { return reflectance; }

private:
    Color albedo;
    float specular;
    float reflectance;
};
