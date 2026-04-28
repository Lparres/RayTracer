#include "PointLight.h"
#include <cmath>

PointLight::PointLight(glm::vec3 pos, Color c)
: Light(c, pos, true)
{

}

// Implementa un modelo de iluminación Blinn-Phong
Color PointLight::shade(Ray r, HitInfo hit) {

    glm::vec3 lightDir = glm::normalize(posOrDir - hit.p);

    // diffuse lighting
    float intensity = std::max(0.f, glm::dot(hit.normal, lightDir));
    Color diffuse = color * intensity * hit.material->get_albedo();

    // specular
    glm::vec3 viewDir = glm::normalize(-r.direction());
    glm::vec3 halfVector = glm::normalize( lightDir + viewDir);
    float specularIntensity = std::max(0.f , glm::dot( hit.normal , halfVector ));
    specularIntensity = pow( specularIntensity , hit.material->get_specular() ); // 30 es intensidad del specular, mover al material
    Color specular = color * specularIntensity ;

    return diffuse + specular;
}

glm::vec3 PointLight::getShadowDir(glm::vec3 pos) {
    return glm::normalize(posOrDir - pos);
}