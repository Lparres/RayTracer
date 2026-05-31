#include "HDRIEnvironment.h"
#include "Texture.h"
#include <math.h>
#define _USE_MATH_DEFINES

HDRIEnvironment::HDRIEnvironment(std::shared_ptr<Texture> map)
: environmentMap(map)
{

}

Color HDRIEnvironment::sample(const glm::vec3& rayDir) const {
    float u = 0.5f + std::atan2(rayDir.z, rayDir.x) / (2.0f*M_PI);
    float v = 0.5f + std::asin(rayDir.y) / M_PI;
    return environmentMap->sample({u,v});
}