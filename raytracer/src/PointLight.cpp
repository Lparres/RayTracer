#include "PointLight.h"
#include <cmath>

PointLight::PointLight(glm::vec3 pos, Color c) :
    Light(c, pos, true)
{}

// Implementa un modelo de iluminación Blinn-Phong
Color PointLight::computeLighting(const Ray& incoming, const HitInfo& hit) const {

    glm::vec3 lightDir = glm::normalize(posOrDir - hit.p);

    // diffuse lighting
    float intensity = std::max(0.f, glm::dot(hit.normal, lightDir));
    Color diffuse = color * intensity * hit.material->getAlbedo();

    // specular
    glm::vec3 viewDir = glm::normalize(-incoming.direction());
    glm::vec3 halfVector = glm::normalize(lightDir + viewDir);
    float specularIntensity = std::max(0.f , glm::dot(hit.normal , halfVector));
    specularIntensity = std::pow(specularIntensity , hit.material->getSpecular());
    Color specular = color * specularIntensity ;

    return diffuse + specular;
}

glm::vec3 PointLight::getShadowDir(const glm::vec3& pos) const {
    return glm::normalize(posOrDir - pos);
}
