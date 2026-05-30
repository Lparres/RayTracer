#include "BlinnPhongMaterial.h"
#include "glm/geometric.hpp"
#include "HitInfo.h"
#include "Ray.h"
#include <algorithm>
#include <cmath>

static constexpr float EPSILON = 1e-7f;
static constexpr float PI = 3.14159265358979f;

BlinnPhongMaterial::BlinnPhongMaterial(Color albedo, float shininess, float reflectance, float ks)
    : _albedoTexture(ConstantTexture::createTexture(albedo))
    , _shininess(shininess)
    , _reflectance(reflectance)
    , _ks(glm::clamp(ks, 0.f, 1.f))
{}

BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<Texture> albedoTexture, float shininess, float reflectance, float ks)
    : _albedoTexture(albedoTexture)
    , _shininess(shininess)
    , _reflectance(reflectance)
    , _ks(glm::clamp(ks, 0.f, 1.f))
{}

Color BlinnPhongMaterial::evaluateDirect(const glm::vec3& wi, const glm::vec3& wo, const HitInfo&hit) const {

    // Difuso normalizado
    Color diffuse = albedo(hit.uv) * (1.f / PI);

    // Especular normalizado
    glm::vec3 halfVector = glm::normalize(wi + wo);
    float NdotH = std::max(glm::dot(hit.normal, halfVector), 0.f);
    float normFactor = (_shininess + 8.f) / (8.f * PI);
    float spec = std::pow(NdotH, _shininess) * normFactor;
    Color specular = Color(spec);

    // Conservación de energía
    float kd = 1.f - _ks;
    return kd * diffuse + _ks * specular;
}

bool BlinnPhongMaterial::scatter(const Ray& incoming, const HitInfo& hit, Color& attenuation, Ray& scattered) const {
    glm::vec3 reflectDir = glm::reflect(incoming.direction(), hit.normal);
    scattered = Ray(hit.p, reflectDir);
    attenuation = Color(_reflectance);

    return (attenuation.r + attenuation.g + attenuation.b) > EPSILON;
}
