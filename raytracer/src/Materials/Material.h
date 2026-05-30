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

    // Devuelve 1.0 si no hay textura AO — el renderer lo aplica al término ambiental
    virtual float     ambientOcclusion(UV uv = {}) const;

    // Devuelve la normal perturbada si hay normal map, o hit.normal si no hay
    // El renderer llama a esto UNA SOLA VEZ antes de cualquier cálculo
    virtual glm::vec3 shadingNormal(const HitInfo& hit) const;

protected:
    // Helper compartido: transforma una normal de tangent space a world space via TBN
    static glm::vec3 perturbNormal(const HitInfo& hit, const Texture& normalMap);
};
