#include "PointLight.h"
#include "glm/geometric.hpp"
#include <cmath>

PointLight::PointLight(glm::vec3 pos, Color c)
: Light(c)
, position(pos)
{

}

// Implementa un modelo de iluminación Blinn-Phong
Color PointLight::shade(Ray r, HitInfo hit) {

    glm::vec3 lightDir = glm::normalize(position - hit.p);

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