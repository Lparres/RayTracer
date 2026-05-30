#include "PointLight.h"
#include "glm/geometric.hpp"
#include <cmath>

PointLight::PointLight(glm::vec3 pos, Color c) :
    Light(c, true),
    position(pos)
{}

glm::vec3 PointLight::getWi(const glm::vec3& hitPoint) const {
    return glm::normalize(position - hitPoint);
}

Light::ShadowRay PointLight::getShadowRay(const glm::vec3& hitPoint) const {
    const glm::vec3 toLight = position - hitPoint;
    const float distance = glm::length(toLight);
    const glm::vec3 direction = distance > 0.0f ? toLight / distance : glm::vec3(0.0f);

    return { Ray(hitPoint, direction), distance };
}
