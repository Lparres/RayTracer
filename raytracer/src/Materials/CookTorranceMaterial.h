#pragma once

#include <memory>
#include "Material.h"
#include "Color.h"
#include "Texture.h"
#include "ConstantTexture.h"

// Modelo de iluminación físicamente realista (PBR).
// Combina un lóbulo difuso Lambertiano con un lóbulo especular Cook-Torrance.
class CookTorranceMaterial : public Material {
public:
    // albedo:    color base de la superficie
    // roughness: rugosidad [0 = espejo perfecto, 1 = completamente difuso]
    // metallic:  factor metálico [0 = dieléctrico, 1 = metal puro]
    explicit CookTorranceMaterial(
        Color albedo,
        float roughness = 0.5f,
        float metallic  = 0.0f
    );

    explicit CookTorranceMaterial(
        std::shared_ptr<Texture> albedoTexture,
        float roughness = 0.5f,
        float metallic  = 0.0f
    );

    virtual ~CookTorranceMaterial() = default;

    Color albedo(UV uv = {}) const override { return _albedoTexture->sample(uv); }

    Color evaluateDirect(const glm::vec3& wi, const glm::vec3& wo, const HitInfo& hit) const override;
    bool  scatter(const Ray& incoming, const HitInfo& hit, Color& attenuation, Ray& scattered) const override;

private:
    // D — Distribución normal Trowbridge-Reitz GGX
    static float D_GGX(const glm::vec3& n, const glm::vec3& h, float alpha);

    // F — Aproximación de Schlick para Fresnel
    static Color F_Schlick(const glm::vec3& h, const glm::vec3& v, const Color& F0);

    // G — Smith Schlick-GGX (autosombreado de microsuperficies)
    static float G_Smith(const glm::vec3& n, const glm::vec3& v,
                         const glm::vec3& l, float roughness);
    static float G_SchlickGGX(float NdotV, float k);

    std::shared_ptr<Texture> _albedoTexture;
    float _roughness;
    float _metallic;
};
