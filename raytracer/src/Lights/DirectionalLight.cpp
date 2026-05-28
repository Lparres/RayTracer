#include "DirectionalLight.h"
#include "glm/geometric.hpp"
#include <cmath>
#include <limits>

DirectionalLight::DirectionalLight(glm::vec3 dir, Color c) :
    Light(c, glm::normalize(dir), false)
{}

Light::ShadowRay DirectionalLight::getShadowRay(const glm::vec3& hitPoint) const {
    const glm::vec3 direction = -posOrDir;
    return { Ray(hitPoint, direction), std::numeric_limits<float>::infinity() };
}
