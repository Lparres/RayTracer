#include "DirectionalLight.h"
#include "glm/geometric.hpp"
#include <cmath>
#include <limits>

DirectionalLight::DirectionalLight(glm::vec3 dir, Color c, float intensity)
    : Light(c, intensity, false)
    , direction(glm::normalize(dir))
{}

Light::LightContribution DirectionalLight::getLightContribution(const glm::vec3& hitPoint) const
{
    return {
        { Ray(hitPoint, -direction), std::numeric_limits<float>::infinity() },
        -direction,
        color * intensity
    };
}
