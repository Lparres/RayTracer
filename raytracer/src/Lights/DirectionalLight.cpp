#include "DirectionalLight.h"
#include "glm/geometric.hpp"
#include <cmath>
#include <limits>

DirectionalLight::DirectionalLight(glm::vec3 dir, Color c) :
    Light(c, false),
    direction(glm::normalize(dir))
{}

glm::vec3 DirectionalLight::getWi(const glm::vec3&) const {
    return -direction;
}

Light::ShadowRay DirectionalLight::getShadowRay(const glm::vec3& hitPoint) const {
    const glm::vec3 direction = -direction;
    return { Ray(hitPoint, direction), std::numeric_limits<float>::infinity() };
}
