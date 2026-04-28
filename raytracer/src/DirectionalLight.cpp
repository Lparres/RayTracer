#include "DirectionalLight.h"
#include "glm/geometric.hpp"
#include <cmath>

DirectionalLight::DirectionalLight(glm::vec3 dir, Color c) 
: Light(c, false)
, direction(glm::normalize(dir))
{

}

// Implementa un modelo de iluminación Blinn-Phong
Color DirectionalLight::shade(Ray r, HitInfo hit) {

    glm::vec3 lightDir = glm::normalize(-direction);

    // diffuse lighting
    float intensity = std::max(0.f, glm::dot(hit.normal, lightDir));
    Color diffuse = color * intensity * hit.material->get_albedo();

    // specular
    glm::vec3 viewDir = glm::normalize(-r.direction());
    glm::vec3 halfVector = glm::normalize( lightDir + viewDir);
    float specularIntensity = std::max(0.f , glm::dot( hit.normal , halfVector ));
    specularIntensity = pow( specularIntensity , 30 ); // 30 es intensidad del specular, mover al material
    Color specular = color * specularIntensity ;

    return diffuse + specular;
}