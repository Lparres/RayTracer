#include "BlinnPhongMaterial.h"
#include "glm/geometric.hpp"
#include "HitInfo.h"
#include "Ray.h"
#include <cmath>

BlinnPhongMaterial::BlinnPhongMaterial(Color albedo, float specular, float reflectance)
    : _albedoTexture(ConstantTexture::createTexture(albedo))
    , _specular(specular)
    , _reflectance(reflectance)
{}

BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<Texture> albedoTexture, float specular, float reflectance)
    : _albedoTexture(albedoTexture)
    , _specular(specular)
    , _reflectance(reflectance)
{}

Color BlinnPhongMaterial::evaluateDirect(const glm::vec3& wi, const glm::vec3& wo, const HitInfo&hit) const {
    // diffuse lighting
    // float intensity = std::max(0.f, glm::dot(hit.normal, wi)); // ahora calculado fuera, término de Lambert
    Color diffuse = albedo(hit.uv);

    // specular
    glm::vec3 halfVector = glm::normalize(wi + wo);
    float specularIntensity = std::max(0.f , glm::dot(hit.normal , halfVector));
    specularIntensity = std::pow(specularIntensity , specular());
    Color specular = Color(1.f) * specularIntensity ;

    return diffuse + specular;
}

bool BlinnPhongMaterial::scatter(const Ray& incoming, const HitInfo& hit, Color& attenuation, Ray& scattered) const {
    glm::vec3 reflectDir = glm::reflect(incoming.direction(), hit.normal);
    scattered = Ray(hit.p, reflectDir);
    attenuation = Color(_reflectance);

    return (attenuation.r + attenuation.g + attenuation.b) > EPSILON;
}
