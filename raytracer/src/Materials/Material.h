#pragma once

#include <memory>
#include "Color.h"
#include "Texture.h"

class Ray;
class HitInfo;

// Material representa las propiedades ópticas de una superficie.
class Material {
public:
    virtual ~Material() = default;
    
    // Evalúa el color del material emitido en la dirección wo, bajo una luz directa incidiendo en la dirección -wi
    virtual Color evaluateDirect(const glm::vec3& wi, const glm::vec3& wo, const HitInfo& hit) const = 0;

    // Calcula el rayo reflejado si lo hay, y su atenuación
    virtual bool scatter(const Ray& incoming, const HitInfo& hit, Color& attenuation, Ray& scattered) const = 0;

    // Para el cálculo de la luz ambiente
    virtual Color albedo(UV uv = {}) const = 0;
};
