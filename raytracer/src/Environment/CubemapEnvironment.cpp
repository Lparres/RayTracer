#include "CubemapEnvironment.h"
#include "Texture.h"
#include "glm/geometric.hpp"

CubemapEnvironment::CubemapEnvironment(
        std::shared_ptr<Texture> top,
        std::shared_ptr<Texture> left,
        std::shared_ptr<Texture> front,
        std::shared_ptr<Texture> right,
        std::shared_ptr<Texture> back,
        std::shared_ptr<Texture> bottom    
) 
: topTexture(top)
, leftTexture(left)
, frontTexture(front)
, rightTexture(right)
, backTexture(back)
, bottomTexture(bottom)
{

}

Color CubemapEnvironment::sample(const glm::vec3& rayDir) const {
    glm::vec3 dir = glm::normalize(rayDir);
    glm::vec3 abs = glm::abs(dir);
    if(abs.x >= abs.y && abs.x >= abs.z) {
        if(dir.x > 0) return rightTexture->sample(normalizeUV(dir.z/abs.x, dir.y/abs.x));
        else return leftTexture->sample(normalizeUV(-dir.z/abs.x, dir.y/abs.x));
    }
    else if(abs.y >= abs.x && abs.y >= abs.z) {
        if(dir.y > 0) return topTexture->sample(normalizeUV(dir.x/abs.y, dir.z/abs.y));
        else return bottomTexture->sample(normalizeUV(dir.x/abs.y, -dir.z/abs.y));
    }
    else {
        if(dir.z > 0) return frontTexture->sample(normalizeUV(-dir.x/abs.z, dir.y/abs.z));
        else return backTexture->sample(normalizeUV(dir.x/abs.z, dir.y/abs.z));
    }
}

UV CubemapEnvironment::normalizeUV(float u, float v) const {
    return UV{
        (u + 1.0f) * 0.5f,
        (v + 1.0f) * 0.5f
    };
}