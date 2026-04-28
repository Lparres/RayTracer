#include "DirectionalLight.h"
#include "glm/geometric.hpp"
#include <cmath>
#include <limits>

DirectionalLight::DirectionalLight(glm::vec3 dir, Color c) :
    Light(c, glm::normalize(dir), false)
{}

// Implementa un modelo de iluminación Blinn-Phong
Color DirectionalLight::computeLighting(const Ray& incoming, const HitInfo& hit) const {

    // diffuse lighting
    float intensity = std::max(0.f, glm::dot(hit.normal, -posOrDir));
    Color diffuse = color * intensity * hit.material->getAlbedo();

    // specular
    glm::vec3 viewDir = glm::normalize(-incoming.direction());
    glm::vec3 halfVector = glm::normalize(-posOrDir + viewDir);
    float specularIntensity = std::max(0.f , glm::dot(hit.normal , halfVector));
    specularIntensity = std::pow(specularIntensity , hit.material->getSpecular());
    Color specular = color * specularIntensity ;

    return diffuse + specular;
}

Light::ShadowRay DirectionalLight::getShadowRay(const glm::vec3& hitPoint) const {
    const glm::vec3 direction = -posOrDir;
    return { Ray(hitPoint, direction), std::numeric_limits<float>::infinity() };
}
