#include "CookTorranceMaterial.h"
#include "glm/geometric.hpp"
#include "HitInfo.h"
#include "Ray.h"
#include "ONB.h"
#include <cmath>

static constexpr float PI      = 3.14159265358979323846f;
static constexpr float EPSILON = 1e-7f;

CookTorranceMaterial::CookTorranceMaterial(Color albedo, float roughness, float metallic)
    : _albedoTexture   (ConstantTexture::createTexture(albedo))
    , _roughnessTexture(ConstantTexture::createTexture(Color(roughness)))
    , _metallicTexture (ConstantTexture::createTexture(Color(metallic)))
{}

CookTorranceMaterial::CookTorranceMaterial(std::shared_ptr<Texture> albedo,
                                            float roughness, float metallic)
    : _albedoTexture   (std::move(albedo))
    , _roughnessTexture(ConstantTexture::createTexture(Color(roughness)))
    , _metallicTexture (ConstantTexture::createTexture(Color(metallic)))
{}

CookTorranceMaterial::CookTorranceMaterial(std::shared_ptr<Texture> albedo,
                                            std::shared_ptr<Texture> roughness,
                                            std::shared_ptr<Texture> metallic)
    : _albedoTexture   (std::move(albedo))
    , _roughnessTexture(std::move(roughness))
    , _metallicTexture (std::move(metallic))
{}

float CookTorranceMaterial::ambientOcclusion(UV uv) const
{
    return _aoTexture ? _aoTexture->sample(uv).r : 1.0f;
}

glm::vec3 CookTorranceMaterial::shadingNormal(const HitInfo& hit) const
{
    return _normalMap ? perturbNormal(hit, *_normalMap) : hit.normal;
}

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
    return alpha2 / (PI * inner * inner + EPSILON);
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

glm::vec3 CookTorranceMaterial::VNDF_GGX(const glm::vec3& woLocal, float alpha, float U1, float U2)
{
    // Calculado en espacio local

    // Configuración del hemisferio (Elipsoide => hemisferio)
    glm::vec3 Vh = glm::normalize(glm::vec3(alpha * woLocal.x, alpha * woLocal.y, woLocal.z));

    // Construimos la base
    float lensq = Vh.x * Vh.x + Vh.y * Vh.y;

    glm::vec3 T1 = lensq > 0.f 
        ? glm::vec3(-Vh.y, Vh.x, 0.f) * glm::inversesqrt(lensq)
        : glm::vec3(1.f, 0.f, 0.f);

    glm::vec3 T2 = glm::cross(Vh, T1);

    // Sampleamos el area proyectada del hemisferio
    float r   = glm::sqrt(U1);
    float phi = 2.f * PI * U2;
    float t1 = r * std::cos(phi);
    float t2 = r * std::sin(phi);
    float s = 0.5f * (1.f + Vh.z);
    t2 = (1.f - s) * glm::sqrt(1.f - t1*t1) + s * t2;

    // Reproyectamos al hemisferio
    glm::vec3 Nh = t1*T1 + t2*T2 + std::sqrt(std::max(0.f, 1.f - t1*t1 - t2*t2)) * Vh;

    // Hemisferio => elipsoide
    glm::vec3 Ne = glm::normalize(glm::vec3(
                alpha * Nh.x,
                alpha * Nh.y,
                std::max(0.f, Nh.z)));

    return Ne;
}


// ─── BRDF completa ────────────────────────────────────────────────────────────

Color CookTorranceMaterial::evaluateDirect(const glm::vec3 &wi, const glm::vec3 &wo,
                                           const HitInfo &hit) const
{
    const float roughness = glm::clamp(_roughnessTexture->sample(hit.uv).r, 0.f, 1.f);
    const float metallic  = glm::clamp(_metallicTexture->sample(hit.uv).r,  0.f, 1.f);
    Color albedoBase = albedo(hit.uv);

    const glm::vec3& n = hit.normal;
    const glm::vec3  h = glm::normalize(wi + wo);

    // Remapping perceptual: alpha = roughness^2 da una percepción más lineal
    const float alpha = roughness * roughness;

    // F0: reflectancia base en incidencia normal
    //   Dieléctrico típico - 0.04  (plástico, piedra, madera...)
    //   Metal              - albedo (los metales tienen reflectancia coloreada)

    Color F0 = Color(0.04f) * (1.f - metallic) + albedoBase * metallic;

    // Evaluar los tres términos
    const float Dval = D_GGX  (n, h, alpha);
    const Color Fval = F_Schlick(h, wo, F0);
    const float Gval = G_Smith (n, wo, wi, roughness);

    // Lóbulo especular: DFG / (4 · (n·wo) · (n·wi))
    float NdotWo = std::max(glm::dot(n, wo), 0.f);
    float NdotWi = std::max(glm::dot(n, wi), 0.f);
    Color specular = (Dval * Fval * Gval) * (1.f / (4.f * NdotWo * NdotWi + EPSILON));

    // Lóbulo difuso: kd · albedo / PI
    // kd = (1−F)·(1−metallic): los metales no tienen componente difusa
    Color kd      = (Color(1.f) - Fval) * (1.f - metallic);
    Color diffuse = kd * albedoBase * (1.f / PI);

    return diffuse + specular;
}

// ─── Reflexión indirecta ──────────────────────────────────────────────────────

bool CookTorranceMaterial::scatter(const Ray& incoming, const HitInfo& hit,
                                    Color& attenuation, Ray& scattered) const
{
    const float roughness  = glm::clamp(_roughnessTexture->sample(hit.uv).r, 0.f, 1.f);
    const float metallic   = glm::clamp(_metallicTexture->sample(hit.uv).r,  0.f, 1.f);
    const Color albedoBase = albedo(hit.uv);
    const Color F0 = Color(0.04f) * (1.f - metallic) + albedoBase * metallic;

    // Rayo especular (determinista)
    ONB basis(hit.normal);
    const glm::vec3 wo = -glm::normalize(incoming.direction());
    const glm::vec3 woLocal = basis.worldToLocal(wo);

    const float alpha = roughness * roughness;
    std::uniform_real_distribution<float> dist(0.f, 1.f);
    const float U1 = dist(_rng);
    const float U2 = dist(_rng);
    
    glm::vec3 mLocal = VNDF_GGX(woLocal, alpha, U1, U2); 
    const glm::vec3 m = glm::normalize(basis.localToWorld(mLocal)); // normal de la microfaceta

    // Escogemos rayo especular o difuso de forma estocástica basada en el término de Fresnel para simular la mezcla entre ambos
    Color F = F_Schlick(m, wo, F0);

    // Probabilidad de especular (basada en el promedio RGB del término de Fresnel)
    // Se limita a [0.05, 0.95] para evitar dividir por 0 en casos extremos.
    float probSpecular = glm::clamp((F.r + F.g + F.b) / 3.0f, 0.05f, 0.95f);
    float probDiffuse = 1.0f - probSpecular;

    if (dist(_rng) < probSpecular) {
        const glm::vec3 reflectDir = glm::reflect(-wo, m);
        
        float NdotWi = std::max(glm::dot(hit.normal, reflectDir), 0.f);
        float G1wi = G_SchlickGGX(NdotWi, alpha); // aproximación

        // Tomar el camino especular puro
        scattered = Ray(hit.p, reflectDir);
        attenuation = (F * G1wi) / probSpecular;
    } else {
        // Rayo difuso Lambertiano: normal + vector unitario aleatorio
        const glm::vec3 randomUnit = glm::normalize(glm::vec3(_gauss(_rng), _gauss(_rng), _gauss(_rng)));
        const glm::vec3 lambertDir = (glm::length(hit.normal + randomUnit) < 1e-4f)
                                ? hit.normal
                                : glm::normalize(hit.normal + randomUnit);
                                
        // Tomar el camino difuso puro
        scattered = Ray(hit.p, lambertDir);
        Color kd = (Color(1.f) - F) * (1.f - metallic);
        attenuation = (kd * albedoBase) / probDiffuse;
    }

    return (attenuation.r + attenuation.g + attenuation.b) > EPSILON;
}
