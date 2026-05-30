#include "CookTorranceMaterial.h"
#include "glm/geometric.hpp"
#include "HitInfo.h"
#include "Ray.h"
#include <cmath>

static constexpr float PI      = 3.14159265358979323846f;

CookTorranceMaterial::CookTorranceMaterial(Color albedo, float roughness, float metallic)
    : _albedoTexture(ConstantTexture::createTexture(albedo))
    , _roughness(glm::clamp(roughness, 0.f, 1.f))
    , _metallic (glm::clamp(metallic,  0.f, 1.f))
{}

CookTorranceMaterial::CookTorranceMaterial(std::shared_ptr<Texture> albedoTexture,
                                            float roughness, float metallic)
    : _albedoTexture(std::move(albedoTexture))
    , _roughness(glm::clamp(roughness, 0.f, 1.f))
    , _metallic (glm::clamp(metallic,  0.f, 1.f))
{}

// ─── D: Trowbridge-Reitz GGX ──────────────────────────────────────────────────
//
//         alpha^2
//  D = ─────────────────────────────
//       PI · ((n·h)^2·(alpha^2−1) + 1)^2

float CookTorranceMaterial::D_GGX(const glm::vec3& n, const glm::vec3& h, float alpha)
{
    float NdotH  = std::max(glm::dot(n, h), 0.f);
    float alpha2 = alpha * alpha;
    float inner  = NdotH * NdotH * (alpha2 - 1.f) + 1.f;
    return alpha2 / (PI * inner * inner + Material::EPSILON);
}

// ─── F: Fresnel — aproximación de Schlick ────────────────────────────────────
//
//  F = F0 + (1 − F0) · (1 − h·v)^5

Color CookTorranceMaterial::F_Schlick(const glm::vec3& h, const glm::vec3& v, const Color& F0)
{
    float HdotV  = std::max(glm::dot(h, v), 0.f);
    float factor = std::pow(1.f - HdotV, 5.f);
    return F0 + (Color(1.f) - F0) * factor;
}

// ─── G: Smith Schlick-GGX ────────────────────────────────────────────────────
//
//  G = G1(v) · G1(l)
//
//             n·v
//  G1(v) = ─────────────────    k = (roughness+1)^2 / 8  [iluminación directa]
//           (n·v)·(1−k) + k

float CookTorranceMaterial::G_SchlickGGX(float NdotV, float k)
{
    return NdotV / (NdotV * (1.f - k) + k + EPSILON);
}

float CookTorranceMaterial::G_Smith(const glm::vec3& n, const glm::vec3& v,
                                     const glm::vec3& l, float roughness)
{
    float r = roughness + 1.f;
    float k = (r * r) / 8.f;                          // remapping para luz directa

    float NdotV = std::max(glm::dot(n, v), 0.f);
    float NdotL = std::max(glm::dot(n, l), 0.f);

    return G_SchlickGGX(NdotV, k) * G_SchlickGGX(NdotL, k);
}

// ─── BRDF completa ────────────────────────────────────────────────────────────

Color CookTorranceMaterial::evaluateDirect(const glm::vec3& wi, const glm::vec3& wo,
                                            const HitInfo& hit) const
{
    const glm::vec3& n = hit.normal;
    const glm::vec3  h = glm::normalize(wi + wo);

    // Remapping perceptual: alpha = roughness^2 da una percepción más lineal
    const float alpha = _roughness * _roughness;

    // F0: reflectancia base en incidencia normal
    //   Dieléctrico típico - 0.04  (plástico, piedra, madera...)
    //   Metal              - albedo (los metales tienen reflectancia coloreada)
    Color baseAlbedo = albedo(hit.uv);
    Color F0 = Color(0.04f) * (1.f - _metallic) + baseAlbedo * _metallic;

    // Evaluar los tres términos
    const float Dval = D_GGX  (n, h, alpha);
    const Color Fval = F_Schlick(h, wo, F0);
    const float Gval = G_Smith (n, wo, wi, _roughness);

    // Lóbulo especular: DFG / (4 · (n·wo) · (n·wi))
    float NdotWo = std::max(glm::dot(n, wo), 0.f);
    float NdotWi = std::max(glm::dot(n, wi), 0.f);
    Color specular = (Dval * Fval * Gval) * (1.f / (4.f * NdotWo * NdotWi + EPSILON));

    // Lóbulo difuso: kd · albedo / PI
    // kd = (1−F)·(1−metallic): los metales no tienen componente difusa
    Color kd      = (Color(1.f) - Fval) * (1.f - _metallic);
    Color diffuse = kd * baseAlbedo * (1.f / PI);

    return diffuse + specular;
}

// ─── Reflexión indirecta ──────────────────────────────────────────────────────

bool CookTorranceMaterial::scatter(const Ray& incoming, const HitInfo& hit,
                                    Color& attenuation, Ray& scattered) const
{
    // Reflexión especular para la componente indirecta.
    // Usamos reflexión especular pura ponderada por F0 y rugosidad.
    glm::vec3 reflectDir = glm::reflect(incoming.direction(), hit.normal);
    scattered = Ray(hit.p, reflectDir);

    // Metales: reflexión coloreada por el albedo, atenuada por la rugosidad
    // Dieléctricos: reflexión débil (4%), también atenuada por rugosidad
    Color baseAlbedo = albedo(hit.uv);
    Color F0 = Color(0.04f) * (1.f - _metallic) + baseAlbedo * _metallic;
    attenuation = F0 * (1.f - _roughness);

    return (attenuation.r + attenuation.g + attenuation.b) > Material::EPSILON;
}
