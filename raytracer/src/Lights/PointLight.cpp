#include "PointLight.h"
#include "glm/geometric.hpp"
#include <cmath>

PointLight::PointLight(glm::vec3 pos, Color c, float intensity) :
    Light(c, intensity, true),
    position(pos)
{}

Light::LightContribution PointLight::getLightContribution(const glm::vec3& hitPoint) const
{
    const glm::vec3 toLight  = position - hitPoint;
    const float     distance = glm::length(toLight);
    const glm::vec3 wi       = distance > 0.f ? toLight / distance : glm::vec3(0.f);
    const float     d2       = distance * distance;
    const Color     Li       = d2 > 0.f ? color * intensity / d2 : color * intensity;

    return { { Ray(hitPoint, wi), distance }, wi, Li };
}
