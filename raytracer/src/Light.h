#pragma once

#include "Ray.h"
#include "Color.h"
#include "HitInfo.h"

class Light {
public:
    ~Light() = default;

    virtual Color shade(Ray r, HitInfo hit) = 0;

protected:
    glm::vec3 position;
    Color color;

    Light(glm::vec3 pos, Color c) : position(pos), color(c) {}
};