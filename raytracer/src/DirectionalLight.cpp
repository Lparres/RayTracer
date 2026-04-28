#include "DirectionalLight.h"
#include "glm/geometric.hpp"
#include <cmath>

DirectionalLight::DirectionalLight(glm::vec3 dir, Color c) :
    Light(c, glm::normalize(dir), false)
{}

// Implementa un modelo de iluminación Blinn-Phong
Color DirectionalLight::shade(Ray r, HitInfo hit) {

    // diffuse lighting
    float intensity = std::max(0.f, glm::dot(hit.normal, -posOrDir));
    Color diffuse = color * intensity * hit.material->getAlbedo();

    // specular
    glm::vec3 viewDir = glm::normalize(-r.direction());
    glm::vec3 halfVector = glm::normalize( -posOrDir + viewDir);
    float specularIntensity = std::max(0.f , glm::dot( hit.normal , halfVector ));
    specularIntensity = pow( specularIntensity , hit.material->getSpecular() );
    Color specular = color * specularIntensity ;

    return diffuse + specular;
}
