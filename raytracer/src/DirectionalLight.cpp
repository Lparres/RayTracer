#include "DirectionalLight.h"
#include "glm/geometric.hpp"
#include <cmath>

DirectionalLight::DirectionalLight(glm::vec3 dir, Color c) 
: Light(glm::vec3(1,1,0), c) // posicion hardcodeada
, direction(glm::normalize(dir))
{

}

Color DirectionalLight::shade(Ray r, HitInfo hit) {
    // diffuse lighting
    float intensity = std::max(0.f, glm::dot(hit.normal, direction));
    Color diffuse = color * intensity * hit.material->get_albedo();

    // specular
    glm::vec3 halfVector = glm::normalize( direction + r.direction());
    float specularIntensity = std::max(0.f , glm::dot( hit.normal , halfVector ));
    specularIntensity = pow( specularIntensity , 100 ); // 100 es intensidad del specular, mover al material
    Color specular = color * specularIntensity ;

    return diffuse + specular;
}