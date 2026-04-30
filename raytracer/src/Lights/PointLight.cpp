#include "PointLight.h"
#include "glm/geometric.hpp"
#include <cmath>

PointLight::PointLight(glm::vec3 pos, Color c) :
    Light(c, pos, true)
{}

// Implementa un modelo de iluminación Blinn-Phong
Color PointLight::computeLighting(const Ray& incoming, const HitInfo& hit) const {

    glm::vec3 lightDir = glm::normalize(posOrDir - hit.p);

    // diffuse lighting
    float intensity = std::max(0.f, glm::dot(hit.normal, lightDir));
    Color diffuse = color * intensity * hit.material->albedo(hit.uv);

    // specular
    glm::vec3 viewDir = glm::normalize(-incoming.direction());
    glm::vec3 halfVector = glm::normalize(lightDir + viewDir);
    float specularIntensity = std::max(0.f , glm::dot(hit.normal , halfVector));
    specularIntensity = std::pow(specularIntensity , hit.material->specular());
    Color specular = color * specularIntensity ;

    return diffuse + specular;
}

Light::ShadowRay PointLight::getShadowRay(const glm::vec3& hitPoint) const {
    const glm::vec3 toLight = posOrDir - hitPoint;
    const float distance = glm::length(toLight);
    const glm::vec3 direction = distance > 0.0f ? toLight / distance : glm::vec3(0.0f);

    return { Ray(hitPoint, direction), distance };
}
